// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "UI/Inventory/InventorySlotWidget.h"
#include "Components/ScrollBox.h"

void UInventoryWidget::InitializeInventory(UInventoryComponent* InInventory)
{
	// 초기화하는 인벤토리가 현재 인벤토리와 같으면 초기화 불필요
	if (Inventory == InInventory) return;

	if (Inventory)
	{
		// 이전 인벤토리의 업데이트 이벤트 등록 해제
		Inventory->OnInventoryUpdated.RemoveDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	}

	// 인벤토리 컴포넌트를 새로 연결
	Inventory = InInventory;

	if (Inventory)
	{
		// 인벤토리의 업데이트 이벤트 등록
		Inventory->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::OnInventoryUpdated);
	}

	// 스크롤 박스를 비우고 슬롯 위젯을 재생성
	RefreshInventory();
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 위젯이 생성되면 초기화
	RefreshInventory();
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
	RefreshInventory();
}

void UInventoryWidget::RefreshInventory()
{
	// 예외 처리
	if (!Inventory || !InventoryScrollBox || !SlotWidgetClass) return;

	// 스크롤 박스 초기화
	InventoryScrollBox->ClearChildren();

	// 인벤토리의 슬롯 데이터 가져오기
	const TArray<FInventorySlot>& Slots = Inventory->Slots;

	// 인벤토리 슬롯의 최대 용량(60칸)만큼 순회
	for (int32 Index = 0; Index < Slots.Num(); ++Index)
	{
		// 새 인벤토리 슬롯 위젯을 생성
		UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, SlotWidgetClass);
		if (SlotWidget)
		{
			// 슬롯 위젯 설정 함수 (아이템 데이터와 UI 연동)
			SlotWidget->SetupSlot(Inventory, Index);
			InventoryScrollBox->AddChild(SlotWidget);
		}
	}
}
