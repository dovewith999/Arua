// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Inventory/InventoryComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "Engine/World.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// 현재 보유 재화(골드) 초기화
	Golds = 0;

	// 최대 슬롯 수 만큼 초기 슬롯 배열 생성
	Slots.SetNumZeroed(MaxSlots);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

int32 UInventoryComponent::AddItem(UDA_ItemDefinition* ItemDef, int32 Amount)
{
	if (!ItemDef || Amount <= 0) return 0;

	// 남은 추가할 아이템 개수
	int32 Remaining = Amount;

	// #1: 먼저 같은 아이템이 있는 슬롯에 추가할 수 있는 만큼 모두 추가
	for (FInventorySlot& Slot : Slots)
	{
		// 해당 슬롯의 아이템 인스턴스가 동일하고, 스택이 가득차지 않은 경우
		if (Slot.ItemDefinition == ItemDef && !Slot.IsFull())
		{
			// 해당 슬롯에 아이템을 추가할 수 있는 만큼 추가함
			Remaining -= Slot.AddQuantity(Remaining);
			if (Remaining <= 0) break;
		}
	}

	// #2: 빈 슬롯에 새로운 스택을 생성
	if (Remaining > 0)
	{
		// 빈 슬롯을 찾음
		for (FInventorySlot& Slot : Slots)
		{
			if (Slot.IsEmpty())
			{
				// 해당 슬롯을 추가할 아이템으로 초기화
				Slot.ItemDefinition = ItemDef;

				// 아이템의 최대 겹침 수량만큼 추가
				const int32 AddAmount = FMath::Min(ItemDef->MaxStackSize, Remaining);
				Slot.Quantity = AddAmount;
				Remaining -= AddAmount;

				// 남은 추가할 아이템 개수를 모두 소진할 때 까지 반복
				if (Remaining <= 0) break;
			}
		}
	}

	// #3: 인벤토리 업데이트 이벤트 브로드캐스트
	if (Amount != Remaining) OnInventoryUpdated.Broadcast();

	// 실제로 추가된 아이템 개수 반환
	return Amount - Remaining;
}

void UInventoryComponent::UseItem(int32 SlotIndex)
{
	// 사용 슬롯이 비어있으면 종료
	if (!Slots.IsValidIndex(SlotIndex)) return;

	// 사용 슬롯의 아이템 데이터 인스턴스를 가져옴
	FInventorySlot& Slot = Slots[SlotIndex];
	if (Slot.IsEmpty()) return;

	// 아이템 효과 적용 (ex. 회복약이라면 캐릭터 체력 회복, 장비라면 장착 처리 등)
	if (Slot.ItemDefinition)
	{
		Slot.ItemDefinition->ApplyEffect(GetOwner());
	}

	// 사용 후 수량 감소
	Slot.Quantity -= 1;

	// 모두 사용했으면 해당 슬롯 초기화
	if (Slot.Quantity <= 0)
	{
		Slot.ItemDefinition = nullptr;
		Slot.Quantity = 0;
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::SplitStack(int32 SlotIndex, int32 SplitAmount)
{
	// 분리하려는 슬롯이 비어있으면 종료
	if (!Slots.IsValidIndex(SlotIndex)) return -1;

	// 분리할 슬롯의 아이템 데이터 인스턴스를 가져옴
	FInventorySlot& Slot = Slots[SlotIndex];

	// 예외 처리
	if (Slot.IsEmpty() || SplitAmount <= 0 || SplitAmount >= Slot.Quantity) return -1;

	// 빈 슬롯 찾기
	for (int32 i = 0; i < Slots.Num(); ++i)
	{
		if (Slots[i].IsEmpty())
		{
			// 빈 슬롯을 현재 아이템으로 초기화
			Slots[i].ItemDefinition = Slot.ItemDefinition;

			// 분리한 아이템 스택만큼으로 초기화
			Slots[i].Quantity = SplitAmount;
			Slot.Quantity -= SplitAmount;

			// 인벤토리 업데이트 이벤트 브로드캐스트
			OnInventoryUpdated.Broadcast();

			// 분리된 새 슬롯 인덱스를 반환
			return i;
		}
	}

	return -1;
}

void UInventoryComponent::RemoveItem(int32 SlotIndex, int32 RemoveAmount)
{
	// 제거하려는 슬롯이 비어있으면 종료
	if (!Slots.IsValidIndex(SlotIndex)) return;

	// 제거할 슬롯의 아이템 데이터 인스턴스를 가져옴
	FInventorySlot& Slot = Slots[SlotIndex];
	if (Slot.IsEmpty()) return;

	// 전체 제거 혹은 제거할 수량이 제거할 슬롯의 수보다 많은 경우
	if (RemoveAmount <= 0 || RemoveAmount >= Slot.Quantity)
	{
		// 전체 제거
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

void UInventoryComponent::MoveItem(int32 FromIndex, int32 ToIndex)
{
	// 두 슬롯 중 하나라도 유효하지 않거나, 두 슬롯이 서로 같은 경우 종료
	if (!Slots.IsValidIndex(FromIndex) || !Slots.IsValidIndex(ToIndex) || FromIndex == ToIndex) return;

	// 옮겨질 슬롯, 옮길 슬롯의 아이템 데이터 인스턴스를 가져옴
	FInventorySlot& From = Slots[FromIndex];
	FInventorySlot& To = Slots[ToIndex];
	if (From.IsEmpty()) return;

	// #1: 옮길 슬롯이 빈 슬롯이면 그대로 이동
	if (To.IsEmpty())
	{
		To = From;
		From.ItemDefinition = nullptr;
		From.Quantity = 0;
	}
	// #2: 두 슬롯의 아이템이 같은 아이템이고, 옮길 슬롯의 스택이 가득 차지 않은 경우
	else if (To.ItemDefinition == From.ItemDefinition && !To.IsFull())
	{
		// 합칠 수 있는 수량 확인
		const int32 MoveAmount = FMath::Min(From.Quantity, To.ItemDefinition->MaxStackSize - To.Quantity);
		To.Quantity += MoveAmount;
		From.Quantity -= MoveAmount;
		if (From.Quantity <= 0)
		{
			From.ItemDefinition = nullptr;
			From.Quantity = 0;
		}
	}
	// #3: 두 슬롯의 아이템이 서로 다른 아이템이면 슬롯 교환
	else
	{
		Swap(From, To);
	}

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::DropItem(int32 SlotIndex)
{
	// 드롭하려는 슬롯이 비어있으면 종료
	if (!Slots.IsValidIndex(SlotIndex)) return;

	// 드롭할 슬롯의 아이템 데이터 인스턴스를 가져옴
	FInventorySlot& Slot = Slots[SlotIndex];
	if (Slot.IsEmpty()) return;

	// UI에서 확인 다이얼로그를 띄운 후 확정되면 아래를 실행
	// 예시로 바로 제거 처리
	// 실제 게임에서는 월드에 드랍 오브젝트를 생성해 떨어뜨릴 수 있음
	Slot.ItemDefinition = nullptr;
	Slot.Quantity = 0;
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::AddGolds(int32 Amount)
{
	// 추가 개수만큼 골드에 더함
	Golds += Amount;

	// 인벤토리 업데이트 이벤트 브로드캐스트
	OnInventoryUpdated.Broadcast();
}

bool UInventoryComponent::SpendGolds(int32 Amount)
{
	// 현재 보유 재화(골드)가 충분한 경우
	if (Golds >= Amount)
	{
		Golds -= Amount;

		// 인벤토리 업데이트 이벤트 브로드캐스트
		OnInventoryUpdated.Broadcast();
		return true;
	}

	return false;
}
