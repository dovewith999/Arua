// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "GameFramework/Actor.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 재화 초기화
	Golds = 0;

	// 인벤토리 슬롯 배열 공간 초기화 (reserve)
	Slots.SetNumZeroed(MaxSlots);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& i : TestStartItems)
	{
		AddItem(i, 1);;
	}
}

//int32 UInventoryComponent::AddItem(UDA_ItemDefinition* ItemDef, int32 Amount)
//{
//	// 예외 처리
//	if (!ItemDef || Amount <= 0) return 0;
//
//	// 추가할 아이템 수량으로 추가해야 할 아이템 수량 초기화
//	// 여러 슬롯에 나눠 추가될 수 있기 떄문에, 차감하는 방식으로 구현
//	int32 Remaining = Amount;
//	int32 Added = 0;
//
//	// 인벤토리 슬롯 배열을 순회하며 아이템을 추가할 슬롯을 확보
//	for (FInventorySlot& InventorySlot : Slots)
//	{
//		// 해당 슬롯이 추가하려는 아이템과 같고, 최대 수량이 아닌 경우
//		if (InventorySlot.ItemDefinition == ItemDef && !InventorySlot.IsFull())
//		{
//			// 해당 슬롯에 추가할 수 있는 만큼 추가 후, 추가해야 할 아이템 수량 차감
//			// 해당 슬롯에 추가할 수 있는 만큼 추가 후, 추가한 개수 저장
//			//Added += InventorySlot.AddQuantity(Amount - Added);
//
//			Remaining -= InventorySlot.AddQuantity(Remaining);
//			if (Remaining <= 0) return 0;
//			//if (Added >= Amount) return Added;
//		}
//	}
//
//	// 새 슬롯 생성
//	if (Remaining > 0)
//	{
//		// 인벤토리 슬롯 배열에서 비어있는 슬롯을 확보
//		for (FInventorySlot& InventorySlot : Slots)
//		{
//			if (InventorySlot.IsEmpty())
//			{
//				// 해당 슬롯의 아이템 정의를 추가하는 아이템으로 초기화
//				InventorySlot.ItemDefinition = ItemDef;
//
//				// 해당 슬롯의 수량을 초기화. 추가하려는 아이템의 최대 수량을 넘지 않도록 Min 계산
//				int32 AddAmount = FMath::Min(ItemDef->MaxStackSize, Remaining);
//				InventorySlot.Quantity = AddAmount;
//
//				// 추가해야 할 아이템 수량 차감
//				Remaining -= AddAmount;
//				if (Remaining <= 0) return 0;
//			}
//		}
//	}
//
//	// 하나라도 아이템을 추가한 경우
//	if (Amount != Remaining)
//	{
//		// 인벤토리 업데이트 이벤트 브로드캐스트
//		OnInventoryUpdated.Broadcast();
//	}
//
//	// 추가한 수량만큼 반환
//	return Amount - Remaining;
//}

int32 UInventoryComponent::AddItem(UDA_ItemDefinition* ItemDef, int32 Amount)
{
	// 예외 처리
	if (!ItemDef || Amount <= 0) return 0;

	// 추가한 아이템 개수
	int32 Added = 0;

	// 인벤토리 슬롯 배열을 순회하며 아이템을 추가할 슬롯을 확보
	for (FInventorySlot& InventorySlot : Slots)
	{
		// 해당 슬롯이 추가하려는 아이템과 같고, 최대 수량이 아닌 경우
		if (InventorySlot.ItemDefinition == ItemDef && !InventorySlot.IsFull())
		{
			// 해당 슬롯에 추가하고 추가한 개수만큼 저장
			Added += InventorySlot.AddQuantity(Amount - Added);
			if (Added >= Amount) 
			{
				// 인벤토리 업데이트 이벤트 브로드캐스트
				OnInventoryUpdated.Broadcast();
				return Added;
			}
		}
	}

	// 새 슬롯 생성
	if (Added < Amount)
	{
		// 인벤토리 슬롯 배열에서 비어있는 슬롯을 확보
		for (FInventorySlot& InventorySlot : Slots)
		{
			if (InventorySlot.IsEmpty())
			{
				// 해당 슬롯의 아이템 정의를 추가하는 아이템으로 초기화
				InventorySlot.ItemDefinition = ItemDef;

				// 해당 슬롯의 수량을 초기화. 추가하려는 아이템의 최대 수량을 넘지 않도록 Min 계산
				int32 AddAmount = FMath::Min(ItemDef->MaxStackSize, Amount - Added);
				InventorySlot.Quantity = AddAmount;

				// 추가해야 할 아이템 수량 차감
				Added += AddAmount;
				if (Added >= Amount) 
				{
					// 인벤토리 업데이트 이벤트 브로드캐스트
					OnInventoryUpdated.Broadcast();
					return Added;
				}
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


void UInventoryComponent::UseItem(int32 SlotIndex)
{
	// 사용하려는 인덱스의 인벤토리 슬롯이 유효하지 않으면 사용 실패
	if (!Slots.IsValidIndex(SlotIndex)) return;

	// 사용하려는 인벤토리 슬롯의 아이템 데이터 가져오기
	FInventorySlot& InventorySlot = Slots[SlotIndex];
	if (InventorySlot.IsEmpty()) return;

	if (InventorySlot.ItemDefinition)
	{
		// 사용자에게 아이템 효과 적용
		InventorySlot.ItemDefinition->ApplyEffect(GetOwner());
	}

	// 아이템 사용 후, 수량 감소
	InventorySlot.Quantity -= 1;

	// 아이템 수량이 0이면, 해당 슬롯을 빈 슬롯으로 초기화
	if (InventorySlot.Quantity <= 0)
	{
		InventorySlot.ItemDefinition = nullptr;
		InventorySlot.Quantity = 0;
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::BundleUseItem(int32 SlotIndex, int32 Amount)
{
	// 사용하려는 인덱스의 인벤토리 슬롯이 유효하지 않으면 사용 실패
	if (!Slots.IsValidIndex(SlotIndex) || Amount <= 0) return;

	// 사용하려는 인벤토리 슬롯의 아이템 데이터 가져오기
	FInventorySlot& InventorySlot = Slots[SlotIndex];
	if (InventorySlot.IsEmpty()) return;

	// 사용할 개수를 초기화
	int32 UseCount = FMath::Min(Amount, InventorySlot.Quantity);

	// 사용 개수만큼 반복적으로 아이템 사용
	for (int32 i = 0; i < UseCount; ++i)
	{
		if (InventorySlot.IsEmpty()) break;

		// 아이템 효과 적용
		if (InventorySlot.ItemDefinition)
		{
			InventorySlot.ItemDefinition->ApplyEffect(GetOwner());
		}

		// 개수 차감
		InventorySlot.Quantity -= 1;

		// 아이템 수량이 0이면, 해당 슬롯을 빈 슬롯으로 초기화
		if (InventorySlot.Quantity <= 0)
		{
			InventorySlot.ItemDefinition = nullptr;
			InventorySlot.Quantity = 0;
			break;
		}
	}

	// 최종적으로 한번만 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::SplitStack(int32 SlotIndex, int32 SplitAmount)
{
	// 나누려는 인덱스의 인벤토리 슬롯이 유효하지 않으면 분리 실패
	if (!Slots.IsValidIndex(SlotIndex)) return -1;

	// 나누려는 인벤토리 슬롯의 아이템 데이터 가져오기
	FInventorySlot& Source = Slots[SlotIndex];
	if (Source.IsEmpty() || SplitAmount <= 0 || SplitAmount >= Source.Quantity) return -1;

	// 나눈 후 저장할 수 있는 슬롯 확보
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsEmpty())
		{
			// 해당 슬롯의 아이템과 수량을 초기화
			Slots[i].ItemDefinition = Source.ItemDefinition;
			Slots[i].Quantity = SplitAmount;

			// 나눈 수량만큼 나눈 슬롯에서 차감
			Source.Quantity -= SplitAmount;

			// 인벤토리 업데이트 이벤트 브로드캐스트
			OnInventoryUpdated.Broadcast();

			// 나눠서 저장한 슬롯 반환
			return i;
		}
	}

	return -1;
}

void UInventoryComponent::RemoveItem(int32 SlotIndex, int32 RemoveAmount)
{
	// 제거하려는 인덱스의 인벤토리 슬롯이 유효하지 않으면 제거 실패
	if (!Slots.IsValidIndex(SlotIndex)) return;

	// 제거하려는 인벤토리 슬롯의 아이템 데이터 가져오기
	FInventorySlot& InventorySlot = Slots[SlotIndex];
	if (InventorySlot.IsEmpty()) return;

	// 모두 지우는 경우, 해당 슬롯을 빈 슬롯으로 초기화
	if (RemoveAmount <= 0 || RemoveAmount >= InventorySlot.Quantity)
	{
		InventorySlot.ItemDefinition = nullptr;
		InventorySlot.Quantity = 0;
	}
	else
	{
		InventorySlot.Quantity -= RemoveAmount;
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::MoveItem(int32 FromIndex, int32 ToIndex)
{
	// 이동 시작/종료하려는 인덱스의 인벤토리 슬롯이 유효하지 않거나 두 인덱스가 같으면 이동 실패
	if (!Slots.IsValidIndex(FromIndex) || !Slots.IsValidIndex(ToIndex) || FromIndex == ToIndex) return;

	// 이동 시작/종료하려는 인덱스의 아이템 데이터 가져오기
	FInventorySlot& From = Slots[FromIndex];
	FInventorySlot& To = Slots[ToIndex];
	if (From.IsEmpty()) return;

	// 이동하려는 인덱스가 비어있는 경우, From의 데이터를 To로 이동
	if (To.IsEmpty())
	{
		To = From;
		To.ItemDefinition = nullptr;
		To.Quantity = 0;
	}
	// 두 슬롯의 아이템 데이터가 같고, 이동하려는 인덱스의 아이템이 최대 수량이 아닌 경우
	else if (From.ItemDefinition == To.ItemDefinition && !To.IsFull())
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
	// 두 슬롯의 아이템 데이터가 다른 경우, 서로 슬롯을 교체
	else
	{
		Swap(From, To);
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::DropItem(int32 SlotIndex)
{
	// 드롭(삭제)하려는 인덱스의 인벤토리 슬롯이 유효하지 않으면 드롭(삭제) 실패
	if (Slots.IsValidIndex(SlotIndex)) return;

	// 드롭(삭제)하려는 인덱스의 아이템 데이터 가져오기
	FInventorySlot& InventorySlot = Slots[SlotIndex];
	if (InventorySlot.IsEmpty()) return;

	// 해당 슬롯을 빈 슬롯으로 초기화
	InventorySlot.ItemDefinition = nullptr;
	InventorySlot.Quantity = 0;

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
