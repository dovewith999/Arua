// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUDView.h"
#include "HpBarView.h"
#include "BossHpBarView.h"
#include "InteractImage.h"

#include "UI/Model/InteractObjectData.h"
#include "UI/ViewModel/ARInteractObjectViewMode.h"

UPlayerHUDView::UPlayerHUDView()
{
}

void UPlayerHUDView::SetViewModelChildWidget(UViewModelBase* InViewModel)
{
	if (PlayerHpBar == nullptr || InViewModel == nullptr)
	{
		return;
	}

	PlayerHpBar->SetViewModel(InViewModel);
}

void UPlayerHUDView::SetBossViewModel(UViewModelBase* InViewModel)
{
	if (BossHpBar == nullptr || InViewModel == nullptr)
	{
		return;
	}

	BossHpBar->SetViewModel(InViewModel);
	BossHpBar->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerHUDView::InitTurnOff()
{
	BossHpBar->SetVisibility(ESlateVisibility::Hidden);
	InteractImage->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerHUDView::SetCanInteract(bool InCanInteract, FString InObjectName)
{
	InteractModel->SetName(InObjectName);
	InteractImage->SetVisibility(InCanInteract == true ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UPlayerHUDView::NativeConstruct()
{
	Super::NativeConstruct();
	InitTurnOff();

#pragma region // InteractImage ViewModel 세팅
	InteractModel = NewObject<UInteractObjectData>(this);
	UARInteractObjectViewModel* InteractViewModel = NewObject<UARInteractObjectViewModel>(this);
	InteractViewModel->Initialize(InteractModel);
	InteractImage->SetViewModel(InteractViewModel);
#pragma endregion
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
