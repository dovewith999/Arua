// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AruaPlayerController.h"
#include "UI/View/PlayerHUDView.h"
#include "UI/Model/PlayerData.h"
#include "UI/ViewModel/PlayerViewModel.h"

AAruaPlayerController::AAruaPlayerController()
{
	static ConstructorHelpers::FClassFinder<UPlayerHUDView> PlayerHUDRef(TEXT("/Game/Personal/LIM_H_S/UI/WBP_HUD.WBP_HUD_C"));

	if (PlayerHUDRef.Succeeded())
	{
		HUDClass = PlayerHUDRef.Class;
	}
}

void AAruaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUD = Cast<UPlayerHUDView>(CreateWidget(GetWorld(), HUDClass)))
	{
		UPlayerData* PlayerData = NewObject<UPlayerData>();

		ViewModel = NewObject<UPlayerViewModel>();
		ViewModel->Initialize(PlayerData);

		// 뷰모델 세팅
		HUD->SetViewModelChildWidget(ViewModel);

		// HUD 뷰포트에 띄우기
		HUD->AddToViewport();
	}
}
