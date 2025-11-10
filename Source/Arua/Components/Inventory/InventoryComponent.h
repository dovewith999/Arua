// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AruaTypes/StructTypes/AR_ItemStruct.h"
#include "InventoryComponent.generated.h"

// 인벤토리 슬롯 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

/**
 * 액터에 부착 가능한 인벤토리 컴포넌트
 * 아이템 슬롯 60개와 재화 골드를 관리
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	// 인벤토리에 아이템을 추가하는 함수
	/*
	* ItemDef		: 추가할 아이템의 정의
	* Amount		: 추가할 개수
	* @Return		: 실제로 추가된 아이템 개수
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 AddItem(UDA_ItemDefinition* ItemDef, int32 Amount);

	// 아이템 사용 함수
	/*
	* SlotIndex		: 사용 슬롯
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(int32 SlotIndex);

	// 아이템 스택 분리 함수
	/*
	* SlotIndex		: 원본 슬롯
	* SplitAmount	: 분리할 수량
	* @Return		: 분리된 새 슬롯 인덱스 or -1(분리 실패)
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 SplitStack(int32 SlotIndex, int32 SplitAmount);

	// 아이템 제거 함수, 겹친 아이템의 경우 일부 제거 가능
	/*
	* SlotIndex		: 제거 대상 슬롯
	* RemoveAmount	: 제거할 개수 (0이면 전체 제거)
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(int32 SlotIndex, int32 RemoveAmount = 0);

	// 아이템 이동 함수, 두 슬롯의 아이템을 서로 바꾸거나 스택을 합친다.
	/*
	* FromIndex		: 이동 시작 슬롯
	* ToIndex		: 이동 대상 슬롯
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void MoveItem(int32 FromIndex, int32 ToIndex);

	// 인벤토리 영역 밖으로 드롭하여 제거하는 함수
	// UI에서 호출 후, 확인 다이얼로그 표시
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(int32 SlotIndex);

	// 재화(골드) 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	void AddGolds(int32 Amount);

	// 재화(골드) 사용 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	bool SpendGolds(int32 Amount);

protected:
	virtual void BeginPlay() override;

public:
	// 인벤토리 업데이트 이벤트 (UI에서 바인딩)
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	// 최대 슬롯 수 (기본값 60)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots = 60;

	// 슬롯 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Slots;

	// 현재 보유 재화(골드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Golds;
};
