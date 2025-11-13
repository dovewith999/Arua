#pragma once

#include "CoreMinimal.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "AR_InventorySlot.generated.h"

/**
 * 실제 인벤토리에서 한 슬롯을 표현하는 데이터 구조체
 * 기본 아이템 정의(DA_ItemDefinition)와 수량을 보유
 */
USTRUCT(BlueprintType)
struct ARUA_API FInventorySlot
{
	GENERATED_BODY()

	// 기본 아이템 인스턴스 초기화, 수량을 0으로 초기화
	FInventorySlot()
		: ItemDefinition(nullptr), Quantity(0)
	{
	}

	// 기본 아이템 인스턴스 (UDA_ItemDefinition의 인스턴스를 가리킴)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	TObjectPtr<UDA_ItemDefinition> ItemDefinition;

	// 현재 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 Quantity;

	// 이 슬롯에 아이템을 추가 (추가할 수 있는 만큼 반환)
	int32 AddQuantity(int32 Amount)
	{
		// 아이템 인스턴스가 없거나 수량이 0인 경우
		if (!ItemDefinition || Amount <= 0) return 0;

		// 최대 겹침 수량까지 남은 수량
		const int32 Remain = ItemDefinition->MaxStackSize - Quantity;

		// 최대 겹침 수량까지 슬롯에 아이템을 추가
		const int32 Addable = FMath::Min(Remain, Amount);
		Quantity += Addable;

		// 현재 슬롯에 추가할 수 있는 만큼 반환
		return Addable;
	}

	// 해당 슬롯이 비어 있는지 확인하는 함수
	bool IsEmpty() const
	{
		return ItemDefinition == nullptr || Quantity <= 0;
	}

	// 해당 슬롯의 스택이 가득 찼는지 확인하는 함수
	bool IsFull() const
	{
		return ItemDefinition && Quantity >= ItemDefinition->MaxStackSize;
	}
};
