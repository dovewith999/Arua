// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AruaTypes/StructTypes/AR_InventorySlot.h"
#include "InventoryComponent.generated.h"

// 인벤토리 슬롯 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

/**
 * 액터에 부착 가능한 인벤토리 컴포넌트
 * 아이템 슬롯 60개와 재화 금액을 관리하며 아이템 사용 효과를 수행
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARUA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 아이템 추가 함수
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

	// 아이템 묶음 사용 함수
	/*
	* SlotIndex		: 사용 슬롯
	* Amount		: 사용 개수
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BundleUseItem(int32 SlotIndex, int32 Amount);

	// 아이템 나누기 함수
	/*
	* SlotIndex		: 원본 슬롯
	* SplitAmount	: 나눌 수량
	* @Return		: 나눠진 새 슬롯 인덱스 or -1(나누기 실패)
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

	// 아이템 드롭(삭제) 함수
	// 인벤토리 영역 밖으로 드롭하여 제거하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(int32 SlotIndex);

	// 재화(골드) 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	void AddGolds(int32 Amount);

	// 재화(골드) 소모 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	bool SpendGolds(int32 Amount);

public:
	// 인벤토리 슬롯 업데이트 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	// 최대 슬롯 수 (기본값 60)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlots = 60;

	// 인벤토리 슬롯 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> Slots;

	// 현재 보유 재화(골드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Golds;
};
