// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AruaTypes/StructTypes/AR_InventorySlot.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "InventoryComponent.generated.h"

// 인벤토리 슬롯 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

/**
 * 액터에 부착 가능한 인벤토리 컴포넌트
 * 각 카테고리별로 별도의 슬롯 배열을 사용하여 아이템을 관리
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
	* Category		: 아이템 카테고리
	* SlotIndex		: 사용 슬롯
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UseItem(EAR_ItemCategory Category, int32 SlotIndex, int32 Amount);

	// 아이템 나누기 함수
	/*
	* Category		: 아이템 카테고리
	* SlotIndex		: 나눌 원본 슬롯 인덱스
	* SplitAmount	: 나누어 옮길 개수 (원본 수량보다 작아야 함)
	* @Return		: 새로 생성된 슬롯 인덱스, 실패 시 -1
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 SplitStack(EAR_ItemCategory Category, int32 SlotIndex, int32 SplitAmount);

	// 아이템 제거 함수, 겹친 아이템의 경우 일부 제거 가능
	/*
	* Category		: 아이템 카테고리
	* SlotIndex		: 제거 대상 슬롯
	* RemoveAmount	: 제거할 개수 (0이면 전체 제거)
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(EAR_ItemCategory Category, int32 SlotIndex, int32 RemoveAmount = 0);

	// 아이템 이동 함수, 같은 카테고리 내의 아이템을 한 슬롯에서 다른 슬롯으로 옮깁니다. 원본 슬롯은 지워지고 대상 슬롯은 덮어씌워집니다.
	/*
	* Category		: 아이템 카테고리
	* FromIndex		: 이동 시작 슬롯
	* ToIndex		: 이동 대상 슬롯
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void MoveItem(EAR_ItemCategory Category, int32 FromIndex, int32 ToIndex);

	// 아이템 합치기 함수, 같은 카테고리 내에서 두 항목을 합치기 시도, 안되면 스왑
	/*
	* Category		: 아이템 카테고리
	* FromIndex		: 이동 시작 슬롯
	* ToIndex		: 이동 대상 슬롯
	*/
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void MergeItem(EAR_ItemCategory Category, int32 FromIndex, int32 ToIndex);

	// 재화(골드) 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	void AddGolds(int32 Amount);

	// 재화(골드) 소모 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	bool SpendGolds(int32 Amount);

	// 재화(골드) Getter 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory | Gold")
	FORCEINLINE int32 GetGolds() const { return Golds; }

	// 카테고리의 슬롯 배열 const Getter
	const TArray<FInventorySlot>& GetSlotsByCategory(EAR_ItemCategory Category) const;

	// 카테고리의 슬롯 배열 참조 Getter
	TArray<FInventorySlot>& GetSlotsByCategory(EAR_ItemCategory Category);

	// 카테고리의 최대 슬롯 수 반환 함수
	int32 GetMaxSlotsByCategory(EAR_ItemCategory Category) const;

public:
	// 인벤토리 슬롯 업데이트 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	// 최대 장비 슬롯 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxEquipmentSlots = 30;

	// 최대 소모품 슬롯 수 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxConsumableSlots = 60;

	// 최대 퀘스트 슬롯 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxQuestSlots = 30;

	// 한 행의 슬롯 행/열 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 SlotsPerRow = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 SlotsPerColumn = 4;

protected:
	// 각 카테고리별 슬롯 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> EquipmentSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> ConsumableSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventorySlot> QuestSlots;

	// 보유 재화(골드)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Golds;

	// 테스트용: 시작 시 가지고 있을 아이템
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<class UDA_ItemDefinition>> TestStartItems;
};
