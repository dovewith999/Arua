// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "UI/Inventory/InventorySlotWidget.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/PanelSlot.h"

void UInventoryWidget::InitializeInventory(UInventoryComponent* InInventory)
{
	// 인벤토리가 이미 초기화되어 있으면 종료
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

	// 슬롯 위젯 재생성
	RefreshGrid();
}

void UInventoryWidget::SetCategory(EAR_ItemCategory NewInventoryCategory)
{
	// 동일 카테고리면 무시
	if (CurrentCategory == NewInventoryCategory) return;

	// 카테고리 변경 후 페이지 리셋
	CurrentCategory = NewInventoryCategory;
	RefreshGrid();
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯이 생성되면 그리드 갱신
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

void UInventoryWidget::OnInventoryUpdated()
{
	RefreshGrid();
}

void UInventoryWidget::RefreshGrid()
{
	// 예외 처리
	if (!Inventory || !InventoryGridPanel || !SlotWidgetClass) return;

	// 현재 카테고리에 해당하는 슬롯 인덱스만 추려냄
	TArray<int32> FilteredIndices;
	FilteredIndices.Reserve(Inventory->Slots.Num()); // 미리 공간 할당

	// 전체 슬롯 배열을 순회하며 필터링
	for (int32 Index = 0; Index < Inventory->Slots.Num(); ++Index)
	{
		const FInventorySlot& InventorySlot = Inventory->Slots[Index];

		// 카테고리가 동일하고 슬롯이 비어있지 않으며 아이템 정의가 유효한 경우
		if (!InventorySlot.IsEmpty() && InventorySlot.ItemDefinition && InventorySlot.ItemDefinition->ItemCategory == CurrentCategory)
		{
			FilteredIndices.Add(Index);
		}
	}

	// 한 페이지에 표시할 최대 슬롯 수
	const int32 MaxSlotsToShow = SlotsPerRow * SlotsPerColumn;

	// 그리드 초기화
	InventoryGridPanel->ClearChildren();

	// 슬롯 채우기 (아이템이 있는 슬롯부터 채움)
	int32 Count = FMath::Min(MaxSlotsToShow, FilteredIndices.Num());

	// 필터링된 슬롯을 순회하며 위젯 생성 및 배치
	for (int32 i = 0; i < Count; ++i)
	{
		int32 SlotIndex = FilteredIndices[i];

		// 슬롯 위젯 생성
		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			// 슬롯 초기화
			SlotWidget->SetupSlot(Inventory, SlotIndex);

			// 그리드 위치 계산
			int32 Row = i / SlotsPerRow;
			int32 Column = i % SlotsPerRow;

			// 그리드 패널에 추가
			UPanelSlot* PanelSlot = InventoryGridPanel->AddChild(SlotWidget);
			if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(PanelSlot))
			{
				// 행과 열 설정: 그리드 레이아웃 적용
				GridSlot->SetRow(Row);
				GridSlot->SetColumn(Column);
			}
		}
	}

	// 부족한 슬롯은 빈 슬롯으로 채움
	if (bFillEmptySlots)
	{
		for (int32 i = Count; i < MaxSlotsToShow; ++i)
		{
			UInventorySlotWidget* EmptySlotWidget = CreateWidget<UInventorySlotWidget>(this, EmptySlotWidgetClass);
			if (EmptySlotWidget)
			{
				// 빈 슬롯을 표시하기 위해 SetupSlot에 유효하지 않은 인덱스를 전달
				EmptySlotWidget->SetupSlot(nullptr, -1);
				int32 Row = i / SlotsPerRow;
				int32 Column = i % SlotsPerRow;
				UPanelSlot* PanelSlot = InventoryGridPanel->AddChild(EmptySlotWidget);
				if (UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(PanelSlot))
				{
					GridSlot->SetRow(Row);
					GridSlot->SetColumn(Column);
				}
			}
		}
	}
}
