// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDView.h"
#include "HpBarView.h"

UPlayerHUDView::UPlayerHUDView()
{
}

void UPlayerHUDView::SetViewModelChildWidget(UViewModelBase* InViewModel)
{
	if (HpBar == nullptr)
	{
		return;
	}

	HpBar->SetViewModel(InViewModel);
}

void UPlayerHUDView::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHUDView::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPlayerHUDView::BindViewModel_Implementation()
{
	Super::BindViewModel_Implementation();
}

void UPlayerHUDView::UnbindViewModel_Implementation()
{
	Super::UnbindViewModel_Implementation();
}

void UPlayerHUDView::OnViewModelPropertyChanged_Implementation(FName PropertyName)
{
	Super::OnViewModelPropertyChanged_Implementation(PropertyName);

	UE_LOG(LogTemp, Log, TEXT("HUD :View Bind Success"));
}
