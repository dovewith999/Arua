// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/ItemContextMenuWidget.h"

void UInventorySlotWidget::SetupSlot(UInventoryComponent* InInventory, int32 InSlotIndex)
{
	if (!InInventory) return;

	Inventory = InInventory;
	SlotIndex = InSlotIndex;
	UpdateSlotDisplay();
}

void UInventorySlotWidget::UpdateSlotDisplay()
{
	// 예외 처리: 슬롯이 비어있는 경우
	if (!Inventory || !Inventory->Slots.IsValidIndex(SlotIndex))
	{
		// 슬롯이 비어있으면 빈 값으로 초기화
		if (ItemIconImage)
		{
			ItemIconImage->SetBrushFromTexture(nullptr);
		}
		if (QuantityText)
		{
			QuantityText->SetText(FText::GetEmpty());
		}
	}


	// 해당 슬롯의 아이템 데이터 가져오기
	const FInventorySlot InventorySlot = Inventory->Slots[SlotIndex];

	// 해당 슬롯의 아이템 데이터가 있는 경우
	if (InventorySlot.ItemDefinition)
	{
		// #1: 아이템 아이콘 이미지 표시
		// 아이템 데이터의 아이템 아이콘 텍스쳐를 동기 로딩
		if (UTexture2D* ItemIconTexture = InventorySlot.ItemDefinition->ItemIcon.LoadSynchronous())
		{
			if (ItemIconImage)
			{
				// 위젯의 아이템 아이콘 이미지에 할당
				ItemIconImage->SetBrushFromTexture(ItemIconTexture);
			}
		}


		// #2: 아이템 수량 표시
		if (QuantityText)
		{
			// 아이템의 수량이 1보다 크면 수량 표시
			if (InventorySlot.Quantity > 1)
			{
				//QuantityText->SetText( FText::AsNumber(InventorySlot.Quantity));
				QuantityText->SetText(FText::FromString(TEXT("x") + FText::AsNumber(InventorySlot.Quantity).ToString()));
			}
			else
			{
				QuantityText->SetText(FText::GetEmpty());
			}
		}


		// #3: 아이템 툴팁 표시
		// 아이템 이름, 아이템 설명, 아이템 정보(스탯 및 적용 수치)
		const FText& ItemName = InventorySlot.ItemDefinition->ItemName;
		const FText& ItemDescription = InventorySlot.ItemDefinition->ItemDescription;
		FString StatString;


		// #3-1: 아이템 정보 확보하기
		// 아이템 분류별로 아이템 정보를 저장
		switch (InventorySlot.ItemDefinition->ItemCategory)
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

		// 확보한 아이템 정보 문자열 저장, 없는 경우 기본값으로 저장
		FText StatText = StatString.IsEmpty() ? FText::GetEmpty() : FText::FromString(StatString);


		// #3-2: 아이템 설명이 없는 경우, 아이템 이름과 아이템 정보만 표시
		if (ItemDescription.IsEmpty())
		{
			if (StatText.IsEmpty())
			{
				// 아이템 이름
				SetToolTipText(ItemName);
			}
			else
			{
				// 아이템 이름 + 아이템 정보
				FText ToolTip = FText::Format(FText::FromString(TEXT("{0}{1}")), ItemName, StatText);
				SetToolTipText(ToolTip);
			}
		}
		// #3-3: 아이템 설명이 있는 경우, 아이템 정보 여부에 따라 툴팁 표시
		else
		{
			if (StatText.IsEmpty())
			{
				// 아이템 이름 + 아이템 설명
				FText ToolTip = FText::Format(FText::FromString(TEXT("{0}\n{1}")), ItemName, ItemDescription);
				SetToolTipText(ToolTip);
			}
			else
			{
				// 아이템 이름 + 아이템 설명 + 아이템 정보
				FText ToolTip = FText::Format(FText::FromString(TEXT("{0}\n{1}{2}")), ItemName, ItemDescription, StatText);
				SetToolTipText(ToolTip);
			}
		}
	}
	// 해당 슬롯의 아이템 데이터가 없는 경우 모두 빈 값으로 초기화
	else
	{
		if (ItemIconImage)
		{
			ItemIconImage->SetBrushFromTexture(nullptr);
		}
		if (QuantityText)
		{
			QuantityText->SetText(FText::GetEmpty());
		}
		SetToolTipText(FText::GetEmpty());
	}
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	// 마우스가 슬롯 위에 오버 시, 추가 효과 (ex. 하이라이트)
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	// 마우스가 슬롯 위를 떠날 때 처리
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 기본 동작 수행
	FReply Reply = UUserWidget::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// #1: 좌클릭: 드래그 감지
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// 예외 처리
		if (Inventory && Inventory->Slots.IsValidIndex(SlotIndex) && !Inventory->Slots[SlotIndex].IsEmpty())
		{
			return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
		}
	}

	// #2: 우클릭: 컨텍스트 메뉴 위젯 생성 및 표시 (추가 옵션들 [사용], [분리], [삭제] 등)
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 예외 처리
		if (Inventory && Inventory->Slots.IsValidIndex(SlotIndex) && !Inventory->Slots[SlotIndex].IsEmpty() && ItemContextMenuWidgetClass)
		{
			// 아이템 콘텍스트 메뉴 위젯 생성
			UItemContextMenuWidget* ItemContextMenuWidget = CreateWidget<UItemContextMenuWidget>(GetWorld(), ItemContextMenuWidgetClass);
			if (ItemContextMenuWidget)
			{
				// 위젯 초기화
				//ItemContextMenuWidget->InitializeMenu(Inventory, SlotIndex);

				// 마우스 커서 위치에 그림
				ItemContextMenuWidget->AddToViewport();
			}
		}
		return FReply::Handled();
	}

	return Reply;
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 예외 처리
	if (!Inventory || !Inventory->Slots.IsValidIndex(SlotIndex)) return;
	if (Inventory->Slots[SlotIndex].IsEmpty()) return;

	// DragDropOperator 생성
	UDragDropOperation* DragOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragOp)
	{
		DragOp->Payload = this;
		DragOp->DefaultDragVisual = this; // 현재 위젯을 드래그 비주얼로 사용
		DragOp->Pivot = EDragPivot::BottomRight;
		OutOperation = DragOp;
	}

}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 예외 처리
	if (!Inventory) return false;

	// 드롭(놓을)할 슬롯 위젯 가져오기
	if (UInventorySlotWidget* SourceWidget = Cast<UInventorySlotWidget>(InOperation->Payload))
	{
		// 같은 인벤토리여야 이동 가능
		if (SourceWidget->Inventory == Inventory)
		{
			// 슬롯 위젯 이동
			Inventory->MoveItem(SourceWidget->SlotIndex, SlotIndex);
			return true;
		}
	}

	return false;
}
