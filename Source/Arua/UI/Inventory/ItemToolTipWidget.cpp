// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemToolTipWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "AruaTypes/Arua_EnumTypes.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemToolTipWidget::InitializeToolTip(UInventoryComponent* InInventory, int32 InSlotIndex)
{
	// 예외 처리
	if (!InInventory || !InInventory->Slots.IsValidIndex(InSlotIndex) || !InInventory->Slots[InSlotIndex].ItemDefinition) return;

	Inventory = InInventory;
	SlotIndex = InSlotIndex;

	// 해당 슬롯의 아이템 정의 데이터 가져오기
	const UDA_ItemDefinition* ItemDefinition = Inventory->Slots[SlotIndex].ItemDefinition;
	if (!ItemDefinition) return;

	// #1: 아이템 이름 설정
	if (ItemNameText)
	{
		ItemNameText->SetText(ItemDefinition->ItemName);
	}

	// #2: 아이템 아이콘 설정
	if (ItemIconImage)
	{
		if (UTexture2D* ItemIconTexture = ItemDefinition->ItemIcon.LoadSynchronous())
		{
			if (ItemIconImage)
			{
				ItemIconImage->SetBrushFromTexture(ItemIconTexture);
			}
		}
	}

	// #3: 아이템 설명 설정
	if (ItemDescriptionText)
	{
		ItemDescriptionText->SetText(ItemDefinition->ItemDescription);
	}

	// #4: 아이템 카테고리 설정
	if (ItemCategoryText)
	{
		// 아이템 분류 열거형의 DisplayName 가져오기
		FText ItemCategoryDisplayNameByFText = StaticEnum<EAR_ItemCategory>()->GetDisplayNameTextByValue(static_cast<int64>(ItemDefinition->ItemCategory));

		ItemCategoryText->SetText(ItemCategoryDisplayNameByFText);
	}

	// #5: 아이템 수치 정보 (공격력/방어력/회복량 등) 설정
	if (ItemStatText)
	{
		ItemStatText->SetText(SetToolTip(ItemDefinition));
	}
}

FText UItemToolTipWidget::SetToolTip(const UDA_ItemDefinition* InItemDefinition)
{
	// 아이템 카테고리별로 분류
	switch (InItemDefinition->ItemCategory)
	{
		// 장비 아이템의 경우, 공격력/방어력 등을 표시
	case EAR_ItemCategory::Equipment:
		break;

		// 소모품 아이템의 경우, 회복량 정보를 표시
	case EAR_ItemCategory::Consumable:
		break;

		// 그 외 카테고리는 추가 아이템 정보 표시를 안 함
	default: break;
	}

	return FText::GetEmpty();
}
