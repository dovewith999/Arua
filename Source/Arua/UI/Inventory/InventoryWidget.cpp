// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/Button.h"

void UInventoryWidget::InitializeInventory(UInventoryComponent* InInventory)
{
	// 같은 인벤토리이면 종료
	if (Inventory == InInventory) return;

	if (Inventory)
	{
		// 이전 인벤토리의 업데이트 이벤트 바인딩 해제
		Inventory->OnInventoryUpdated.RemoveDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	}

	// 인벤토리 컴포넌트를 새로 연결
	Inventory = InInventory;

	if (Inventory)
	{
		// 인벤토리의 업데이트 이벤트 등록
		Inventory->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	}

	if (NextPageButton)
	{
		// 다음 페이지로 이동하는 이벤트 바인딩
		NextPageButton->OnClicked.AddDynamic(this, &UInventoryWidget::NextPage);
	}

	if (PrevPageButton)
	{
		// 다음 페이지로 이동하는 이벤트 바인딩
		PrevPageButton->OnClicked.AddDynamic(this, &UInventoryWidget::PrevPage);
	}

	// 페이지 초기화 후 그리드 재생성
	CurrentPageIndex = 0;
	RefreshGrid();
}

void UInventoryWidget::SetCategory(EAR_ItemCategory NewCategory)
{
	// 동일 카테고리면 무시
	if (CurrentCategory == NewCategory) return;

	// 카테고리 변경 후 그리드 재생성
	CurrentCategory = NewCategory;
	CurrentPageIndex = 0;
	RefreshGrid();
}

void UInventoryWidget::NextPage()
{
	if (!Inventory) return;

	// 해당 카테고리 인벤토리의 최대 슬롯 수
	const int32 MaxSlots = Inventory->GetMaxSlotsByCategory(CurrentCategory);

	// 페이지 당 표시할 슬롯 수
	const int32 PageCapacity = Inventory->SlotsPerRow * Inventory->SlotsPerColumn;

	// 표시할 슬롯 계산
	const int32 TotalPages = FMath::DivideAndRoundUp(MaxSlots, PageCapacity);

	// 페이지를 넘기고 그리드 재생성
	if (CurrentPageIndex < TotalPages - 1)
	{
		++CurrentPageIndex;
		RefreshGrid();
	}
}

void UInventoryWidget::PrevPage()
{
	if (CurrentPageIndex > 0)
	{
		// 페이지 감소 후 그리드 재생성
		--CurrentPageIndex;
		RefreshGrid();
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);

	// 위젯이 생성되면 그리드 재생성
	RefreshGrid();
}

void UInventoryWidget::NativeDestruct()
{
	if (Inventory)
	{
		// 인벤토리의 업데이트 이벤트 등록 해제
		Inventory->OnInventoryUpdated.RemoveDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	}

	Super::NativeDestruct();
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// 키 입력 이벤트 확인
	const FKey PressedKey = InKeyEvent.GetKey();

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UInventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 마우스 클릭 시 키보드 포커스를 다시 이 위젯으로 설정
	SetKeyboardFocus();

	return FReply::Handled();
}

void UInventoryWidget::OnInventoryUpdated()
{
	RefreshGrid();
}

void UInventoryWidget::RefreshGrid()
{
	// 예외 처리
	if (!Inventory || !SlotGrid || !SlotWidgetClass) return;

	// 그리드 초기화
	SlotGrid->ClearChildren();

	// 해당 카테고리의 슬롯 참조 배열 가져오기
	const TArray<FInventorySlot>& SlotsRef = Inventory->GetSlotsByCategory(CurrentCategory);

	// 해당 카테고리 인벤토리의 최대 슬롯 수
	const int32 MaxSlots = Inventory->GetMaxSlotsByCategory(CurrentCategory);

	// 페이지 당 표시할 슬롯 수
	const int32 PageCapacity = Inventory->SlotsPerRow * Inventory->SlotsPerColumn;

	// 시작 슬롯 인덱스 (현재 페이지 * 페이지 당 표시 슬롯 수)
	const int32 StartIndex = CurrentPageIndex * PageCapacity;

	// 이 페이지에 표시할 슬롯 수
	// (ex. 최대 슬롯 70개, 첫 번째 슬롯 인덱스 = 60번쨰라면, 10개 슬롯만 표시)
	int32 SlotsToShow = FMath::Clamp(MaxSlots - StartIndex, 0, PageCapacity);

	// 새 슬롯 위젯 생성
	for (int32 i = 0; i < SlotsToShow; ++i)
	{
		// 슬롯 인덱스 구하기 (시작 인덱스로부터 계산)
		const int32 SlotIndex = StartIndex + i;

		// 슬롯 위젯 생성
		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			// 빈 슬롯인지 검사 후, 위젯 초기화
			const bool bEmpty = !SlotsRef.IsValidIndex(SlotIndex) || SlotsRef[SlotIndex].IsEmpty();
			SlotWidget->SetupSlot(Inventory, CurrentCategory, SlotIndex, bEmpty);

			// 그리드 위치 계산 (인벤토리에서 한 페이지에 표시할 행/열 고려)
			const int32 Row = i / Inventory->SlotsPerRow;
			const int32 Col = i % Inventory->SlotsPerRow;

			// 그리드 패널에 추가
			if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(SlotGrid->AddChild(SlotWidget)))
			{
				// 그리드 레이아웃 적용 (행과 열 설정)
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Col);
			}
		}
	}

	// 페이지의 나머지 슬롯을 최대 슬롯 수를 넘지 않도록 빈 슬롯으로 채움
	for (int32 i = SlotsToShow; i < PageCapacity; ++i)
	{
		const int32 SlotIndex = StartIndex + i;

		// 최대 슬롯 수를 넘으면 종료
		if (SlotIndex >= MaxSlots) break;

		// // 슬롯 위젯 생성 + 초기화 + 그리드 패널에 추가
		UInventorySlotWidget* EmptyWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (EmptyWidget)
		{
			EmptyWidget->SetupSlot(Inventory, CurrentCategory, SlotIndex, true);
			const int32 Row = i / Inventory->SlotsPerRow;
			const int32 Col = i % Inventory->SlotsPerRow;
			if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(SlotGrid->AddChild(EmptyWidget)))
			{
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Col);
			}
		}
	}
}
