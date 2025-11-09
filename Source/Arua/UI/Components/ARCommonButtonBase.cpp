// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Components/ARCommonButtonBase.h"
#include "CommonTextBlock.h"

void UARCommonButtonBase::SetButtonText(FText InText)
{
	if (!CommonTextBlock_ButtonText || InText.IsEmpty())
	{
		return;
	}

	CommonTextBlock_ButtonText->SetText(bUserUpperCaseForButtonText ? InText.ToUpper() : InText);
}

void UARCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetButtonText(ButtonDisplayText);
}
