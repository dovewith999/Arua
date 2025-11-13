// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractImage.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

UInteractImage::UInteractImage()
{
}

void UInteractImage::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInteractImage::NativeDestruct()
{
	Super::NativeDestruct();
}

void UInteractImage::BindViewModel_Implementation()
{
	Super::BindViewModel_Implementation();
}

void UInteractImage::UnbindViewModel_Implementation()
{
	Super::UnbindViewModel_Implementation();
}

void UInteractImage::OnViewModelPropertyChanged_Implementation(FName PropertyName)
{
	if (PropertyName == "Text" || PropertyName == NAME_None)
	{
		Text_Message->SetText(FText::FromString(TEXT("상호작용")));
	}
}
