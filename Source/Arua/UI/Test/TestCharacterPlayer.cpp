// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Test/TestCharacterPlayer.h"
#include "PlayerViewModel.h"
#include "PlayerData.h"
#include "PlayerHUDView.h"

ATestCharacterPlayer::ATestCharacterPlayer()
{
	static ConstructorHelpers::FClassFinder<UPlayerHUDView> PlayerHUDRef(TEXT("/Game/Personal/LIM_H_S/UI/WBP_HUD.WBP_HUD_C"));
	if (PlayerHUDRef.Succeeded())
	{
		HUDClass = PlayerHUDRef.Class;
	}
}

void ATestCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 뷰모델 생성
	VM = NewObject<UPlayerViewModel>();

	UPlayerData* PlayerData = NewObject<UPlayerData>();

	// 모델 설정
	VM->Initialize(PlayerData);

	HUD = Cast<UPlayerHUDView>(CreateWidget(GetWorld(), HUDClass)); 
	HUD->AddToViewport();
	//HUD->SetViewModel(VM);
	HUD->SetViewModelChildWidget(VM);
}
