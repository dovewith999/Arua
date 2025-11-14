// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "InventoryWidget.generated.h"

/**
 * 인벤토리 UI를 표시하는 위젯
 */
UCLASS()
class ARUA_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 인벤토리 위젯 초기화 함수
	/** 인벤토리 설정 및 이벤트 바인딩 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void InitializeInventory(class UInventoryComponent* InInventory);

	// 인벤토리 카테고리를 변경하고 페이지를 리셋하는 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetCategory(EAR_ItemCategory NewCategory);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 인벤토리 변경 시 호출되는 함수
	UFUNCTION()
	void OnInventoryUpdated();

	// 슬롯 위젯을 재생성하는 함수
	void RefreshGrid();

protected:
	// 아이템 슬롯을 배치할 UniformGridPanel
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> InventoryGridPanel;

	// 슬롯 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UInventorySlotWidget> SlotWidgetClass;

	// 빈 슬롯 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UInventorySlotWidget> EmptySlotWidgetClass;

	// 현재 연결된 인벤토리 컴포넌트
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	// 현재 선택된 카테고리
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EAR_ItemCategory CurrentCategory = EAR_ItemCategory::Equipment;

	// 한 행의 슬롯 수 (열 개수)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	int32 SlotsPerRow;

	// 한 열의 슬롯 수 (행 개수)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	int32 SlotsPerColumn;

	// 빈 슬롯을 표시할지 여부
	// 빈 슬롯도 위젯을 생성해 배경을 표시하고 싶다면 true로 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	bool bFillEmptySlots = true;
};
