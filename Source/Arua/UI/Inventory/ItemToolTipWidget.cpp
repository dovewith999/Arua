// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemToolTipWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "AruaTypes/Arua_EnumTypes.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UItemToolTipWidget::InitializeToolTip(UInventoryComponent* InInventory, EAR_ItemCategory InCategory, int32 InSlotIndex)
{
	// 예외 처리
	if (!InInventory) return;

	Inventory = InInventory;
	Category = InCategory;
	SlotIndex = InSlotIndex;

	// 해당 슬롯 카테고리의 참조 배열 가져오기
	const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

	if (!SlotsRef.IsValidIndex(SlotIndex) || !SlotsRef[SlotIndex].ItemDefinition) return;

	// 해당 슬롯의 아이템 정의 데이터 가져오기
	const UDA_ItemDefinition* ItemDefinition = SlotsRef[SlotIndex].ItemDefinition;
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

void UItemToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/* 포커스를 통한 위젯 생명주기 관리 */
	SetIsFocusable(true);

	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(TakeWidget());
		Mode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(Mode);
	}
}

void UItemToolTipWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	// 위젯이 포커스를 잃을 경우 위젯 제거
	RemoveFromParent();
}

void UItemToolTipWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	// 포커스 경로에서 완전히 빠질 때에도 위젯 제거
	RemoveFromParent();
}
