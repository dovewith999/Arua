// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 인벤토리의 각 슬롯을 표시하는 위젯
 * 아이콘과 수량 표시, 마우스 오버, 드래그 앤 드롭 등 처리
 */
UCLASS()
class ARUA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 인벤토리 참조와 슬롯 인덱스 설정 및 UI 갱신
	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetupSlot(UInventoryComponent* InInventory, int32 InSlotIndex);

protected:
	// 슬롯 UI 갱신 함수
	void UpdateSlotDisplay();

	// 마우스 관련 이벤트 함수
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	// 아이템 아이콘 이미지
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;

	// 아이템 수량 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> QuantityText;

	// 슬롯이 속한 인벤토리
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	// 슬롯 인덱스
	int32 SlotIndex;

	// 우클릭 메뉴 위젯 클래스 (아이템 사용 관련 콘텍스트)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UItemContextMenuWidget> ItemContextMenuWidgetClass;
};
