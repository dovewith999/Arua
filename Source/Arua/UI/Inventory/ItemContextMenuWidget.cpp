// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemContextMenuWidget.h"
#include "UI/Inventory/ItemQuantityPopupWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Inventory/InventoryComponent.h"

void UItemContextMenuWidget::InitializeMenu(UInventoryComponent* InInventory, EAR_ItemCategory InCategory, int32 InSlotIndex)
{
	if (!InInventory) return;

	Inventory = InInventory;
	Category = InCategory;
	SlotIndex = InSlotIndex;

	// 해당 슬롯 카테고리의 참조 배열 가져오기
	const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

	// 해당 슬롯 데이터 가져오기
	const FInventorySlot& InventorySlot = SlotsRef[SlotIndex];

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
		Inventory->UseItem(Category, SlotIndex, 1);
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
		// 아이템 수량 선택 팝업 위젯 설정
		SetUpItemQuantityPopupWidget(PopUp, EItemPopUpAction::BundleUse);
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

	// 해당 슬롯 카테고리의 참조 배열 가져오기
	const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

	// 해당 슬롯 데이터 가져오기
	const FInventorySlot& InventorySlot = SlotsRef[SlotIndex];

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
		// 아이템 수량 선택 팝업 위젯 설정
		SetUpItemQuantityPopupWidget(PopUp, EItemPopUpAction::Split);
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
		// 아이템 수량 선택 팝업 위젯 설정
		SetUpItemQuantityPopupWidget(PopUp, EItemPopUpAction::Remove);
	}

	RemoveFromParent();
}

void UItemContextMenuWidget::OnCancelClicked()
{
	RemoveFromParent();
}

void UItemContextMenuWidget::SetUpItemQuantityPopupWidget(UItemQuantityPopupWidget* PopUpWidget, EItemPopUpAction InAction)
{
	// 위젯 초기화 및 화면에 추가
	PopUpWidget->InitializePopUp(Inventory, Category, SlotIndex, InAction);
	PopUpWidget->AddToViewport();

	// 화면 위치
	int32 ViewportX = 0;
	int32 ViewportY = 0;

	// 플레이어 컨트롤러로부터 뷰포트 크기 가져오기
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->GetViewportSize(ViewportX, ViewportY);
	}

	// 뷰포트 크기의 절반을 사용하여 중심 위치를 계산
	FVector2D CenterPosition(static_cast<float>(ViewportX) * 0.5f, static_cast<float>(ViewportY) * 0.5f);

	// 위젯의 원점을 중앙에 정렬
	PopUpWidget->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));

	// 위젯 위치 설정
	PopUpWidget->SetPositionInViewport(CenterPosition, true);

	PopUpWidget->SetVisibility(ESlateVisibility::Visible);
}
