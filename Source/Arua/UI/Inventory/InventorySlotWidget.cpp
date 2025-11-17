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

void UInventorySlotWidget::SetupSlot(UInventoryComponent* InInventory, EAR_ItemCategory InCategory, int32 InSlotIndex, bool bInEmptySlot)
{
	if (!InInventory) return;

	Inventory = InInventory;
	Category = InCategory;
	SlotIndex = InSlotIndex;
	bIsEmptySlot = bInEmptySlot;

	UpdateSlotDisplay();
}

void UInventorySlotWidget::UpdateSlotDisplay()
{
	if (!Inventory) return;

	// 해당 슬롯 카테고리의 참조 배열 가져오기
	const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

	// 해당 슬롯 데이터 가져오기
	const FInventorySlot& InventorySlot = SlotsRef[SlotIndex];

	// 최대 슬롯 수를 초과했거나 빈 슬롯인 경우 슬롯 위젯을 빈 값으로 초기화
	if (bIsEmptySlot || !SlotsRef.IsValidIndex(SlotIndex))
	{
		if (ItemIconImage)
		{
			if (UTexture2D* IconTexture = EmptyItemIcon.LoadSynchronous())
			{
				if (IconTexture)
				{
					ItemIconImage->SetBrushFromTexture(IconTexture);
				}
			}
		}
		if (QuantityText)
		{
			QuantityText->SetText(FText::GetEmpty());
		}
		return;
	}

	// 해당 슬롯의 아이템 데이터가 있는 경우
	if (InventorySlot.ItemDefinition)
	{
		// #1: 아이템 아이콘 이미지 표시
		// 아이템 데이터의 아이템 아이콘 텍스쳐를 동기 로딩
		if (UTexture2D* IconTexture = InventorySlot.ItemDefinition->ItemIcon.LoadSynchronous())
		{
			if (IconTexture)
			{
				ItemIconImage->SetBrushFromTexture(IconTexture);
			}
		}

		// #2: 아이템 수량 표시
		if (QuantityText)
		{
			if (InventorySlot.Quantity > 1)
			{
				QuantityText->SetText(FText::FromString(TEXT("x") + FString::FromInt(InventorySlot.Quantity)));
			}
			else
			{
				QuantityText->SetText(FText::GetEmpty());
			}
		}
	}
	else
	{
		if (ItemIconImage)
		{
			if (UTexture2D* IconTexture = EmptyItemIcon.LoadSynchronous())
			{
				if (IconTexture)
				{
					ItemIconImage->SetBrushFromTexture(IconTexture);
				}
			}
		}
		if (QuantityText)
		{
			QuantityText->SetText(FText::GetEmpty());
		}
		return;
	}
}

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (ItemToolTipWidgetClass && !bIsEmptySlot)
	{
		if (!ItemToolTipWidgetInstance)
		{
			// 아이템 툴팁 위젯 생성 및 표시
			ItemToolTipWidgetInstance = CreateWidget<UItemToolTipWidget>(GetWorld(), ItemToolTipWidgetClass);
			if (ItemToolTipWidgetInstance)
			{
				// 위젯 초기화 및 화면에 추가
				ItemToolTipWidgetInstance->InitializeToolTip(Inventory, Category, SlotIndex);
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
	if (ItemToolTipWidgetInstance && !bIsEmptySlot)
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
		if (Inventory && !bIsEmptySlot)
		{
			// 해당 슬롯 카테고리의 참조 배열 가져오기
			const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

			if (SlotsRef.IsValidIndex(SlotIndex) && !SlotsRef[SlotIndex].IsEmpty())
			{
				// 드래그 감지
				return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
			}
		}
	}

	// #2: 우클릭: 컨텍스트 메뉴 위젯 생성 및 표시 (추가 옵션들 [사용], [분리], [삭제] 등)
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (!Inventory || bIsEmptySlot)
		{
			return Reply;
		}

		// 해당 슬롯 카테고리의 참조 배열 가져오기
		const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

		if (!SlotsRef.IsValidIndex(SlotIndex) || SlotsRef[SlotIndex].IsEmpty())
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
			ItemContextMenuWidgetInstance->InitializeMenu(Inventory, Category, SlotIndex);
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
	if (!Inventory || bIsEmptySlot) return;

	// 해당 슬롯 카테고리의 참조 배열 가져오기
	const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

	// 예외 처리
	if (!SlotsRef.IsValidIndex(SlotIndex) || SlotsRef[SlotIndex].IsEmpty()) return;

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
	if (!Inventory || !InOperation) return false;

	// 드롭(놓을)할 슬롯 위젯 가져오기
	if (UInventorySlotWidget* SourceWidget = Cast<UInventorySlotWidget>(InOperation->Payload))
	{
		// 동일한 인벤토리 및 카테고리 내에서만 드롭을 허용
		if (bIsEmptySlot && !Inventory->GetSlotsByCategory(Category).IsValidIndex(SlotIndex)) return false;

		// 해당 슬롯 카테고리의 참조 배열 가져오기
		const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(Category);

		// 드롭하는 슬롯이 비어 있으면 이동하고, 그렇지 않으면 아이템 합치기 시도
		const bool bDestEmpty = !SlotsRef.IsValidIndex(SlotIndex) || SlotsRef[SlotIndex].IsEmpty();
		if (bDestEmpty)
		{
			Inventory->MoveItem(Category, SourceWidget->SlotIndex, SlotIndex);
		}
		else
		{
			Inventory->MergeItem(Category, SourceWidget->SlotIndex, SlotIndex);
		}
		return true;
	}

	return false;
}
