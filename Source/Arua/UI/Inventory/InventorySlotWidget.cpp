// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventorySlotWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "AruaTypes/Arua_EnumTypes.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Inventory/ItemContextMenuWidget.h"
#include "UI/Inventory/ItemToolTipWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

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
				QuantityText->SetText(FText::FromString(TEXT("x") + FText::AsNumber(InventorySlot.Quantity).ToString()));
			}
			else
			{
				QuantityText->SetText(FText::GetEmpty());
			}
		}
	}
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (ItemToolTipWidgetClass)
	{
		if (!ItemToolTipWidgetInstance)
		{
			// 아이템 툴팁 위젯 생성 및 표시
			ItemToolTipWidgetInstance = CreateWidget<UItemToolTipWidget>(GetWorld(), ItemToolTipWidgetClass);
			if (ItemToolTipWidgetInstance)
			{
				// 위젯 초기화 및 화면에 추가
				ItemToolTipWidgetInstance->InitializeToolTip(Inventory, SlotIndex);
				ItemToolTipWidgetInstance->AddToViewport();

				// 위젯의 원점을 0으로 조정
				ItemToolTipWidgetInstance->SetAlignmentInViewport(FVector2D(0.f, 0.f));

				// 슬롯의 화면(스크린) 좌표 기준 좌상단 위치/크기 가져오기
				const FVector2D SlotAbsPos = InGeometry.GetAbsolutePosition();
				const FVector2D SlotAbsSize = InGeometry.GetAbsoluteSize();

				// 슬롯의 위젯 크기만큼 더한 위치로 설정
				FVector2D ToolTipPos = SlotAbsPos + FVector2D(SlotAbsSize.X, 0.f);

				// 아이템 툴팁 위젯 위치 설정
				ItemToolTipWidgetInstance->SetPositionInViewport(ToolTipPos, true);

				ItemToolTipWidgetInstance->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}

	// 마우스가 슬롯 위에 오버 시, 추가 효과 (ex. 하이라이트)
	if (ItemHighlightImage)
	{
		ItemHighlightImage->SetColorAndOpacity(FLinearColor(FVector4f(1.f, 1.f, 1.f, 1.f)));
	}
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// 아이템 툴팁 위젯 제거
	if (ItemToolTipWidgetInstance)
	{
		ItemToolTipWidgetInstance->RemoveFromParent();
		ItemToolTipWidgetInstance = nullptr;
	}

	// 마우스가 슬롯 위를 떠날 때 처리
	if (ItemHighlightImage)
	{
		ItemHighlightImage->SetColorAndOpacity(FLinearColor(FVector4f(1.f, 1.f, 1.f, 0.f)));
	}
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
		if (!Inventory || !Inventory->Slots.IsValidIndex(SlotIndex) || Inventory->Slots[SlotIndex].IsEmpty() || !ItemContextMenuWidgetClass)
		{
			return Reply;
		}

		// 아이템 콘텍스트 메뉴 위젯 생성
		if (ItemContextMenuWidgetInstance)
		{
			ItemContextMenuWidgetInstance->RemoveFromParent();
			ItemContextMenuWidgetInstance = nullptr;
		}

		ItemContextMenuWidgetInstance = CreateWidget<UItemContextMenuWidget>(GetWorld(), ItemContextMenuWidgetClass);
		if (ItemContextMenuWidgetInstance)
		{
			// 위젯 초기화 및 화면에 추가
			ItemContextMenuWidgetInstance->InitializeMenu(Inventory, SlotIndex);
			ItemContextMenuWidgetInstance->AddToViewport();

			// 마우스 커서 위치
			float MouseX = 0.f;
			float MouseY = 0.f;

			// 플레이어 컨트롤러로부터 마우스 위치 가져오기
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC && PC->GetMousePosition(MouseX, MouseY))
			{
				// 마우스 위치 저장
				FVector2D MenuPosition(MouseX, MouseY);

				// 위젯의 원점을 0으로 조정
				ItemContextMenuWidgetInstance->SetAlignmentInViewport(FVector2D(0.f, 0.f));

				// 마우스 커서를 고려해서 오프셋 설정
				const FVector2D Offset(1.f, 1.f);

				// 위젯의 뷰포트 위치 설정
				ItemContextMenuWidgetInstance->SetPositionInViewport(MenuPosition + Offset, true);
			}

			ItemContextMenuWidgetInstance->SetVisibility(ESlateVisibility::Visible);
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
		DragOp->Pivot = EDragPivot::CenterCenter;
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
		// 슬롯 위젯 이동
		Inventory->MoveItem(SourceWidget->SlotIndex, SlotIndex);
		return true;


		//// 같은 인벤토리여야 이동 가능
		//if (SourceWidget->Inventory == Inventory)
		//{
		//	// 슬롯 위젯 이동
		//	Inventory->MoveItem(SourceWidget->SlotIndex, SlotIndex);
		//	return true;
		//}
	}

	return false;
}
