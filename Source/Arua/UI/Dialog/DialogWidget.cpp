// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Dialog/DialogWidget.h"
#include "Input/Reply.h"
#include "InputCoreTypes.h"

void UDialogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);     // 포커스 가능
	SetKeyboardFocus();       // 처음 열릴 때 포커스
}

FReply UDialogWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// 키 입력 이벤트 확인
	const FKey PressedKey = InKeyEvent.GetKey();

	// 키 입력 이벤트 델리게이트 브로드캐스트
	if (PressedKey == EKeys::Escape || PressedKey == EKeys::Q)
	{
		OnRequestInput.Broadcast();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UDialogWidget::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// 키 입력 이벤트 확인
	const FKey PressedKey = InKeyEvent.GetKey();

	// 키 입력 이벤트 델리게이트 브로드캐스트
	if (PressedKey == EKeys::Escape || PressedKey == EKeys::Q)
	{
		OnRequestInput.Broadcast();
		return FReply::Handled();
	}

	return Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
}

FReply UDialogWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 마우스 클릭 시 키보드 포커스를 다시 이 위젯으로 설정
	SetKeyboardFocus();

	return FReply::Handled();
}
