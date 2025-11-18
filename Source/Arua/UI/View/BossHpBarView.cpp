// Fill out your copyright notice in the Description page of Project Settings.

#include "BossHpBarView.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/ViewModel/BossViewModel.h"

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

	UBossViewModel* VM = Cast<UBossViewModel>(GetViewModel());

	if (VM == nullptr)
	{
		return;
	}

	if (PropertyName == NAME_None)
	{
		UE_LOG(LogTemp, Log, TEXT("Set None"));

		HpProgressBar->SetPercent(VM->GetHpRatio());
		NameText->SetText(FText::FromString(VM->GetName()));
	}

	if (PropertyName == "Hp" || PropertyName == "MaxHp")
	{
		UE_LOG(LogTemp, Log, TEXT("Set BossHpBar"));
		HpProgressBar->SetPercent(VM->GetHpRatio());
	}

	else if (PropertyName == "Name")
	{
		UE_LOG(LogTemp, Log, TEXT("Set Name"));
		NameText->SetText(FText::FromString(VM->GetName()));
	}

	else if (PropertyName == "IsSensed")
	{
		HpProgressBar->SetPercent(VM->GetHpRatio());
		NameText->SetText(FText::FromString(VM->GetName()));

		SetVisibility(ESlateVisibility::Visible);
	}

	else if (PropertyName == "IsUnsensed")
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
