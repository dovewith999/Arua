// Fill out your copyright notice in the Description page of Project Settings.


#include "HpBarView.h"
#include "Components/ProgressBar.h"
#include "UI/ViewModel/PlayerViewModel.h"

void UHpBarView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHpBarView::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHpBarView::BindViewModel_Implementation()
{
	Super::BindViewModel_Implementation();
}

void UHpBarView::UnbindViewModel_Implementation()
{
	Super::UnbindViewModel_Implementation();
}

void UHpBarView::OnViewModelPropertyChanged_Implementation(FName PropertyName)
{
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpProgressBar")));

	if (UPlayerViewModel* VM = Cast<UPlayerViewModel>(GetViewModel()))
	{
		if (PropertyName == "Hp")
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.0f,
				FColor::Blue,
				FString::Printf(TEXT("Current HP Percent: %f"), HpProgressBar->GetPercent())
			);

			HpProgressBar->SetPercent(1.f);
			GEngine->AddOnScreenDebugMessage(
				-1,
				3.0f,
				FColor::Green,
				FString::Printf(TEXT("Current HP Percent: %f"), HpProgressBar->GetPercent())
			);
		}
	}
}
