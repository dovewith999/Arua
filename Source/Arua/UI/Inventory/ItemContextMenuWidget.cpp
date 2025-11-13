// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemContextMenuWidget.h"
#include "Components/Button.h"
#include "Components/Inventory/InventoryComponent.h"

void UItemContextMenuWidget::InitializeMenu(UInventoryComponent* InInventory, int32 InSlotIndex)
{
	Inventory = InInventory;
	SlotIndex = InSlotIndex;
}

void UItemContextMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 각 버튼에 맞는 이벤트 바인딩
	if (OnUseButton)
	{
		OnUseButton->OnClicked.AddDynamic(this, &UItemContextMenuWidget::OnUseClicked);
	}
	if (OnBundleUseButton)
	{
		OnBundleUseButton->OnClicked.AddDynamic(this, &UItemContextMenuWidget::OnBundleUseClicked);
	}
	if (OnSplitButton)
	{
		OnSplitButton->OnClicked.AddDynamic(this, &UItemContextMenuWidget::OnSplitClicked);
	}
	if (OnRemoveButton)
	{
		OnRemoveButton->OnClicked.AddDynamic(this, &UItemContextMenuWidget::OnRemoveClicked);
	}
	if (OnCancleButton)
	{
		OnCancleButton->OnClicked.AddDynamic(this, &UItemContextMenuWidget::OnCancelClicked);
	}
}

void UItemContextMenuWidget::OnUseClicked()
{
	// 아이템 사용 후, 위젯 제거
	if (Inventory)
	{
		Inventory->UseItem(SlotIndex);
	}
	RemoveFromParent();
}

void UItemContextMenuWidget::OnBundleUseClicked()
{
	// 아이템 사용 후, 위젯 제거
	if (Inventory)
	{
		Inventory->BundleUseItem(SlotIndex, 1);
	}
	RemoveFromParent();
}

void UItemContextMenuWidget::OnSplitClicked()
{
	if (Inventory)
	{
		// 나누려는 슬롯의 데이터 가져오기
		const FInventorySlot& Slots = Inventory->Slots[SlotIndex];
		if (Slots.Quantity > 1)
		{

		}
	}
	RemoveFromParent();
}

void UItemContextMenuWidget::OnRemoveClicked()
{

}

void UItemContextMenuWidget::OnCancelClicked()
{
	RemoveFromParent();
}
