// Fill out your copyright notice in the Description page of Project Settings.

#include "BossHpBarView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossHpBarView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBossHpBarView::NativeDestruct()
{
	Super::NativeDestruct();
}

void UBossHpBarView::BindViewModel_Implementation()
{
	Super::BindViewModel_Implementation();
}

void UBossHpBarView::UnbindViewModel_Implementation()
{
	Super::UnbindViewModel_Implementation();
}

void UBossHpBarView::OnViewModelPropertyChanged_Implementation(FName PropertyName)
{
	Super::OnViewModelPropertyChanged_Implementation(PropertyName);

	if (PropertyName == "Hp")
	{

	}

	else if (PropertyName == "Name")
	{

	}
}
