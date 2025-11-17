// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemToolTipWidget.generated.h"

/**
 * 아이템 슬롯에 마우스 오버 시, 아이템 정보를 나타내는 툴팁 위젯
 */
UCLASS()
class ARUA_API UItemToolTipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 아이템 툴팁 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "ToolTip")
	void InitializeToolTip(class UInventoryComponent* InInventory, EAR_ItemCategory InCategory, int32 InSlotIndex);

	// 툴팁 텍스트 갱신 함수
	FText SetToolTip(const class UDA_ItemDefinition* InItemDefinition);

protected:
	// 아이템 이름
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemNameText;

	// 아이템 아이콘
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UImage> ItemIconImage;

	// 아이템 설명
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemDescriptionText;

	// 아이템 카테고리
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemCategoryText;

	// 아이템 수치 정보 (공격력/방어력/회복량 등)
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ItemStatText;

private:
	// 연동된 인벤토리
	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Inventory;

	// 연동된 인벤토리 슬롯 카테고리
	EAR_ItemCategory Category;

	// 연동된 인벤토리 슬롯 인덱스
	int32 SlotIndex;
};
