// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/ItemQuantityPopupWidget.h"
#include "Components/Inventory/InventoryComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "AruaTypes/Arua_EnumTypes.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"

void UItemQuantityPopupWidget::InitializePopUp(UInventoryComponent* InInventory, int32 InSlotIndex, EItemPopUpAction InAction)
{
	Inventory = InInventory;
	SlotIndex = InSlotIndex;
	Action = InAction;

	if (!Inventory || !Inventory->Slots.IsValidIndex(SlotIndex)) return;

	// 슬롯의 아이템 데이터 가져오기
	const FInventorySlot& InventorySlot = Inventory->Slots[SlotIndex];

	// #1: 팝업 제목 설정하기
	FText Title;
	switch (Action)
	{
		// 묶음 사용 팝업 액션
	case EItemPopUpAction::BundleUse:
		Title = FText::FromString(TEXT("아이템 사용"));
		break;

		// 나누기 팝업 액션
	case EItemPopUpAction::Split:
		Title = FText::FromString(TEXT("아이템 나누기"));
		break;

		// 제거 팝업 액션
	case EItemPopUpAction::Remove:
		Title = FText::FromString(TEXT("아이템 제거"));
		break;

	default:
		Title = FText::GetEmpty();
		break;
	}

	if (TitleText)
	{
		// 팝업 제목 설정
		TitleText->SetText(Title);
	}


	// #2: 아이템 아이콘 이미지 설정
	if (ItemIconImage && InventorySlot.ItemDefinition)
	{
		// 아이템 데이터의 아이템 아이콘 이미지 동기 로딩 후, 아이템 아이콘 이미지 설정
		if (UTexture2D* Icon = InventorySlot.ItemDefinition->ItemIcon.LoadSynchronous())
		{
			ItemIconImage->SetBrushFromTexture(Icon);
		}
	}


	// #3: 현재 수량/최대 수량 표시 설정
	int32 MaxQuantity = InventorySlot.Quantity;
	if (QuantityLabel)
	{
		// 현재 수량 / 최대 수량
		QuantityLabel->SetText(FText::Format(FText::FromString(TEXT("{0} / {1}")), FText::AsNumber(MaxQuantity), FText::AsNumber(MaxQuantity)));
	}


	// #4: 스핀박스 설정
	if (QuantitySpinBox)
	{
		// 스핀박스 기본/최소/최대 값 설정
		QuantitySpinBox->SetMinValue(1.f);
		QuantitySpinBox->SetMaxValue(static_cast<float>(MaxQuantity));
		QuantitySpinBox->SetValue(1.f);

		// 슬라이더를 통한 최소/최대 값 설정
		QuantitySpinBox->Delta = 1.f; // 숫자 이동값 정도
		QuantitySpinBox->SetMinSliderValue(1.f);
		QuantitySpinBox->SetMaxSliderValue(static_cast<float>(MaxQuantity));
	}
}

void UItemQuantityPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 이벤트 바인딩
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UItemQuantityPopupWidget::OnConfirmCliked);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UItemQuantityPopupWidget::OnCancelCliked);
	}
}

void UItemQuantityPopupWidget::OnConfirmCliked()
{
	// 예외 처리
	if (!Inventory || !Inventory->Slots.IsValidIndex(SlotIndex) || !QuantitySpinBox)
	{
		RemoveFromParent();
		return;
	}

	// 스핀 박스로부터 얻은 값을 가져옴
	int32 Amount = FMath::Max(1, static_cast<int32>(QuantitySpinBox->GetValue()));

	// 팝업 액션에 따른 분기 처리
	switch (Action)
	{
		// 아이템 묶음 사용
	case EItemPopUpAction::BundleUse:
		Inventory->BundleUseItem(SlotIndex, Amount);
		break;

		// 아이템 나누기
	case EItemPopUpAction::Split:
		Inventory->SplitStack(SlotIndex, Amount);
		break;

		// 아이템 제거
	case EItemPopUpAction::Remove:
		Inventory->RemoveItem(SlotIndex, Amount);
		break;

	default: break;
	}

	RemoveFromParent();
}

void UItemQuantityPopupWidget::OnCancelCliked()
{
	RemoveFromParent();
}
