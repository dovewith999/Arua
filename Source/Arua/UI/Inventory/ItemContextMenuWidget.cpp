// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemContextMenuWidget.h"
#include "UI/Inventory/ItemQuantityPopupWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Inventory/InventoryComponent.h"

void UItemContextMenuWidget::InitializeMenu(UInventoryComponent* InInventory, int32 InSlotIndex)
{
	Inventory = InInventory;
	SlotIndex = InSlotIndex;

	// 슬롯의 아이템 데이터 가져오기
	const FInventorySlot& InventorySlot = Inventory->Slots[SlotIndex];

	// 아이템 이름 설정
	if (ItemName && InventorySlot.ItemDefinition)
	{
		ItemName->SetText(InventorySlot.ItemDefinition->ItemName);
	}
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
	// 예외 처리
	if (!Inventory || !ItemQuantityPopupWidgetClass)
	{
		RemoveFromParent();
		return;
	}

	// 아이템 수량 선택 팝업 위젯 생성 및 초기화
	UItemQuantityPopupWidget* PopUp = CreateWidget<UItemQuantityPopupWidget>(GetWorld(), ItemQuantityPopupWidgetClass);
	if (PopUp)
	{
		// 아이템 묶음 사용으로 팝업 위젯 초기화 후 그림
		PopUp->InitializePopUp(Inventory, SlotIndex, EItemPopUpAction::BundleUse);
		PopUp->AddToViewport();
	}

	RemoveFromParent();
}

void UItemContextMenuWidget::OnSplitClicked()
{
	// 예외 처리
	if (!Inventory || !ItemQuantityPopupWidgetClass)
	{
		RemoveFromParent();
		return;
	}

	// 나누려는 슬롯의 아이템 데이터 가져오기
	const FInventorySlot& InventorySlot = Inventory->Slots[SlotIndex];

	// 나눌 수 있는 수량이 없다면 아무 동작 없이 메뉴 닫기
	if (InventorySlot.Quantity <= 1)
	{
		RemoveFromParent();
		return;
	}

	// 아이템 수량 선택 팝업 위젯 생성 및 초기화
	UItemQuantityPopupWidget* PopUp = CreateWidget<UItemQuantityPopupWidget>(GetWorld(), ItemQuantityPopupWidgetClass);
	if (PopUp)
	{
		// 아이템 나누기로 팝업 위젯 초기화 후 그림
		PopUp->InitializePopUp(Inventory, SlotIndex, EItemPopUpAction::Split);
		PopUp->AddToViewport();
	}

	RemoveFromParent();
}

void UItemContextMenuWidget::OnRemoveClicked()
{
	// 예외 처리
	if (!Inventory || !ItemQuantityPopupWidgetClass)
	{
		RemoveFromParent();
		return;
	}

	// 아이템 수량 선택 팝업 위젯 생성 및 초기화
	UItemQuantityPopupWidget* PopUp = CreateWidget<UItemQuantityPopupWidget>(GetWorld(), ItemQuantityPopupWidgetClass);
	if (PopUp)
	{
		// 아이템 묶음 사용으로 팝업 위젯 초기화 후 그림
		PopUp->InitializePopUp(Inventory, SlotIndex, EItemPopUpAction::Remove);
		PopUp->AddToViewport();
	}

	RemoveFromParent();
}

void UItemContextMenuWidget::OnCancelClicked()
{
	RemoveFromParent();
}
