// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 재화 초기화
	Golds = 0;

	// 인벤토리 슬롯 배열 초기화 (공간 확보)
	EquipmentSlots.SetNum(MaxEquipmentSlots);
	ConsumableSlots.SetNum(MaxConsumableSlots);
	QuestSlots.SetNum(MaxQuestSlots);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 테스트 아이템 추가
	for (UDA_ItemDefinition* ItemDef : TestStartItems)
	{
		if (ItemDef)
		{
			AddItem(ItemDef, 1);
		}
	}
}

int32 UInventoryComponent::AddItem(UDA_ItemDefinition* ItemDef, int32 Amount)
{
	// 예외 처리
	if (!ItemDef || Amount <= 0) return 0;

	// 추가하려는 아이템 카테고리 가져오기
	const EAR_ItemCategory Category = ItemDef->ItemCategory;

	// 해당 아이템 카테고리의 인벤토리 슬롯 목록 가져오기
	TArray<FInventorySlot>& SlotsRef = GetSlotsByCategory(Category);

	// 추가한 아이템 개수
	int32 Added = 0;

	// 먼저 기존의 같은 아이템 중첩 시도
	// ex. 빨간 물약(x2) -> 빨간 물약(x6)
	for (FInventorySlot& Slot : SlotsRef)
	{
		// 같은 아이템이고, 최대 스택이 아닌 경우
		if (Slot.ItemDefinition == ItemDef && !Slot.IsFull())
		{
			// 추가해야 하는 남은 아이템 개수
			const int32 Remaining = Amount - Added;

			// 해당 슬롯에 더 쌓을 수 있는 개수, 추가해야 하는 남은 아이템 개수 Min 검사
			const int32 Addable = FMath::Min(ItemDef->MaxStackSize - Slot.Quantity, Remaining);

			// 추가하는 수만큼 갱신
			Slot.Quantity += Addable;
			Added += Addable;

			if (Added >= Amount)
			{
				// 인벤토리 업데이트 이벤트 브로드캐스트
				OnInventoryUpdated.Broadcast();
				return Added;
			}
		}
	}

	// 빈 슬롯에 추가
	for (FInventorySlot& Slot : SlotsRef)
	{
		if (Slot.IsEmpty())
		{
			// 해당 빈 슬롯의 아이템 정의를 추가하려는 아이템으로 정의
			Slot.ItemDefinition = ItemDef;

			// 아이템 정의의 최대 스택 수량을 기준으로 추가할 아이템 개수 Min 계산
			const int32 Remaining = Amount - Added;
			const int32 AddAmount = FMath::Min(ItemDef->MaxStackSize, Remaining);

			// 추가하는 수만큼 갱신
			Slot.Quantity = AddAmount;
			Added += AddAmount;

			if (Added >= Amount)
			{
				// 인벤토리 업데이트 이벤트 브로드캐스트
				OnInventoryUpdated.Broadcast();
				return Added;
			}
		}
	}

	// 하나라도 아이템을 추가한 경우
	if (Added > 0)
	{
		// 인벤토리 업데이트 이벤트 브로드캐스트
		OnInventoryUpdated.Broadcast();
	}

	// 추가한 수량만큼 반환
	return Added;
}


void UInventoryComponent::UseItem(EAR_ItemCategory Category, int32 SlotIndex, int32 Amount)
{
	// 해당 아이템 카테고리의 인벤토리 슬롯 목록 가져오기
	TArray<FInventorySlot>& SlotsRef = GetSlotsByCategory(Category);
	if (!SlotsRef.IsValidIndex(SlotIndex)) return;

	// 사용하려는 슬롯의 슬롯 데이터 가져오기
	FInventorySlot& Slot = SlotsRef[SlotIndex];
	if (!Slot.ItemDefinition || Slot.IsEmpty()) return;

	// 실제 사용할 개수(추가 개수 vs 남은 개수 중 최소)
	int32 UseCount = FMath::Min(Amount, Slot.Quantity);

	// 사용하려는 개수만큼 아이템 반복 사용
	for (int32 i = 0; i < UseCount; ++i)
	{
		// 슬롯이 도중에 비워졌으면 방어
		if (Slot.IsEmpty() || !Slot.ItemDefinition) break;

		Slot.ItemDefinition->ApplyEffect(GetOwner());
		Slot.Quantity--;
	}

	// 아이템 남은 개수가 0이면 빈 슬롯으로 초기화
	if (Slot.Quantity <= 0)
	{
		Slot.ItemDefinition = nullptr;
		Slot.Quantity = 0;
	}

	// 최종적으로 한번만 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::SplitStack(EAR_ItemCategory Category, int32 SlotIndex, int32 SplitAmount)
{
	// 해당 아이템 카테고리의 인벤토리 슬롯 목록 가져오기
	TArray<FInventorySlot>& SlotsRef = GetSlotsByCategory(Category);
	if (!SlotsRef.IsValidIndex(SlotIndex)) return -1;

	// 나누려는 슬롯의 슬롯 데이터 가져오기
	FInventorySlot& FromSlot = SlotsRef[SlotIndex];
	if (!FromSlot.ItemDefinition || FromSlot.IsEmpty()) return -1;
	if (SplitAmount <= 0 || SplitAmount >= FromSlot.Quantity) return -1;

	// 새 스택을 넣을 빈 슬롯 찾기
	int32 EmptyIndex = -1;
	for (int32 i = 0; i < SlotsRef.Num(); ++i)
	{
		// 원본 슬롯은 스킵
		if (i == SlotIndex) continue;

		if (SlotsRef[i].IsEmpty())
		{
			EmptyIndex = i;
			break;
		}
	}

	// 빈 슬롯을 못 찾았으면 실패
	if (EmptyIndex == -1)
	{
		return -1;
	}

	// 새 슬롯 설정
	FInventorySlot& NewSlot = SlotsRef[EmptyIndex];
	NewSlot.ItemDefinition = FromSlot.ItemDefinition;
	NewSlot.Quantity = SplitAmount;

	// 원본 수량 감소
	FromSlot.Quantity -= SplitAmount;

	// 원본이 이론상 0이 될 수 없지만, 예외 처리
	if (FromSlot.Quantity <= 0)
	{
		FromSlot.ItemDefinition = nullptr;
		FromSlot.Quantity = 0;
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();

	// 새로 생성된 슬롯 인데스 반환
	return EmptyIndex;
}

void UInventoryComponent::RemoveItem(EAR_ItemCategory Category, int32 SlotIndex, int32 RemoveAmount)
{
	// 해당 아이템 카테고리의 인벤토리 슬롯 목록 가져오기
	TArray<FInventorySlot>& SlotsRef = GetSlotsByCategory(Category);
	if (!SlotsRef.IsValidIndex(SlotIndex)) return;

	// 제거하려는 슬롯의 슬롯 데이터 가져오기
	FInventorySlot& Slot = SlotsRef[SlotIndex];
	if (!Slot.ItemDefinition || Slot.IsEmpty()) return;

	// 모두 제거하는 경우, 빈 슬롯으로 전환
	if (RemoveAmount <= 0 || RemoveAmount >= Slot.Quantity)
	{
		Slot.ItemDefinition = nullptr;
		Slot.Quantity = 0;
	}
	else
	{
		Slot.Quantity -= RemoveAmount;
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::MoveItem(EAR_ItemCategory Category, int32 FromIndex, int32 ToIndex)
{
	// 해당 아이템 카테고리의 인벤토리 슬롯 목록 가져오기
	TArray<FInventorySlot>& SlotsRef = GetSlotsByCategory(Category);
	if (!SlotsRef.IsValidIndex(FromIndex) || !SlotsRef.IsValidIndex(ToIndex) || FromIndex == ToIndex) return;

	// 두 슬롯을 스왑
	SlotsRef.Swap(FromIndex, ToIndex);

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::MergeItem(EAR_ItemCategory Category, int32 FromIndex, int32 ToIndex)
{
	// 해당 아이템 카테고리의 인벤토리 슬롯 목록 가져오기
	TArray<FInventorySlot>& SlotsRef = GetSlotsByCategory(Category);
	if (!SlotsRef.IsValidIndex(FromIndex) || !SlotsRef.IsValidIndex(ToIndex) || FromIndex == ToIndex) return;

	// 이동 시작/종료하려는 인덱스의 아이템 데이터 가져오기
	FInventorySlot& From = SlotsRef[FromIndex];
	FInventorySlot& To = SlotsRef[ToIndex];
	if (From.IsEmpty()) return;

	// 두 슬롯의 아이템 데이터가 같고, 이동하려는 인덱스의 아이템이 최대 수량이 아닌 경우
	if (From.ItemDefinition == To.ItemDefinition && !To.IsFull())
	{
		// 이동(추가)시킬 수 있는 수량 확인 후, 추가
		int32 MoveAmount = FMath::Min(From.Quantity, To.ItemDefinition->MaxStackSize - To.Quantity);
		To.Quantity += MoveAmount;

		// 이동(추가)시킨 만큼 From에서 차감
		From.Quantity -= MoveAmount;

		// From의 남은 수량이 없으면 해당 슬롯을 빈 슬롯으로 초기화
		if (From.Quantity <= 0)
		{
			From.ItemDefinition = nullptr;
			From.Quantity = 0;
		}
	}
	else
	{
		// 두 슬롯을 스왑
		SlotsRef.Swap(FromIndex, ToIndex);
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::AddGolds(int32 Amount)
{
	// 현재 재화(골드) 추가
	Golds += Amount;

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::SpendGolds(int32 Amount)
{
	if (Golds >= Amount)
	{
		// 현재 재화(골드) 차감
		Golds -= Amount;

		// 인벤토리 업데이트 이벤트 브로드캐스트
		OnInventoryUpdated.Broadcast();

		// 재화(골드) 차감 성공 반환
		return true;
	}

	// 재화(골드) 차감 실패 반환
	return false;
}

const TArray<FInventorySlot>& UInventoryComponent::GetSlotsByCategory(EAR_ItemCategory Category) const
{
	switch (Category)
	{
	case EAR_ItemCategory::Equipment:	return EquipmentSlots;
	case EAR_ItemCategory::Consumable:	return ConsumableSlots;
	case EAR_ItemCategory::Quest:		return QuestSlots;
	default:							return EquipmentSlots;
	}
}

TArray<FInventorySlot>& UInventoryComponent::GetSlotsByCategory(EAR_ItemCategory Category)
{
	switch (Category)
	{
	case EAR_ItemCategory::Equipment:	return EquipmentSlots;
	case EAR_ItemCategory::Consumable:	return ConsumableSlots;
	case EAR_ItemCategory::Quest:		return QuestSlots;
	default:							return EquipmentSlots;
	}
}

int32 UInventoryComponent::GetMaxSlotsByCategory(EAR_ItemCategory Category) const
{
	switch (Category)
	{
	case EAR_ItemCategory::Equipment:	return MaxEquipmentSlots;
	case EAR_ItemCategory::Consumable:	return MaxConsumableSlots;
	case EAR_ItemCategory::Quest:		return MaxQuestSlots;
	default:							return 0;
	}
}
