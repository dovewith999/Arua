// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 인벤토리의 각 슬롯을 표시하는 위젯 클래스
 * 아이콘과 수량 표시, 마우스 오버, 드래그 앤 드롭 등 처리
 */
UCLASS()
class ARUA_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 인벤토리 참조와 슬롯 인덱스 설정 및 UI 갱신
	UFUNCTION(BlueprintCallable, Category = "Slot")
	void SetupSlot(UInventoryComponent* InInventory, EAR_ItemCategory InCategory, int32 InSlotIndex, bool bInEmptySlot);

	// 이 슬롯이 빈 슬롯인지 반환하는 함수
	bool GetIsEmptySlot() const { return bIsEmptySlot; }

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

	// 아이템 슬롯 하이트라이트 이미지
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ItemHighlightImage;

	// 현재 연결된 인벤토리 컴포넌트
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	// 슬롯 카테고리
	EAR_ItemCategory Category;

	// 슬롯 인덱스
	int32 SlotIndex;

	// 아이템 콘텍스트 메뉴 위젯 클래스 (아이템 사용 관련 콘텍스트)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UItemContextMenuWidget> ItemContextMenuWidgetClass;

	// 아이템 툴팁 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UItemToolTipWidget> ItemToolTipWidgetClass;

	// 빈 슬롯에 표시될 아이콘
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> EmptyItemIcon;

private:
	// 아이템 콘텍스트 메뉴 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UItemContextMenuWidget> ItemContextMenuWidgetInstance;

	// 아이템 툴팁 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UItemToolTipWidget> ItemToolTipWidgetInstance;

	// 빈 슬롯 여부
	bool bIsEmptySlot = false;
};
