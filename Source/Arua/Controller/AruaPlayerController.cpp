// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AruaPlayerController.h"
#include "UI/View/PlayerHUDView.h"
#include "UI/Model/PlayerData.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "Test/TestCharacterPlayer.h"

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

}

void AAruaPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (HUD = Cast<UPlayerHUDView>(CreateWidget(this, HUDClass)))
	{
		UPlayerData* PlayerData = NewObject<UPlayerData>();
		PlayerData->Initialize();
		ATestCharacterPlayer* OwnerPlayer = Cast<ATestCharacterPlayer>(aPawn);
		PlayerData->BindToAttributeSet(OwnerPlayer->GetAttributeSet());

		ViewModel = NewObject<UPlayerViewModel>();
		ViewModel->Initialize(PlayerData);

		// 뷰모델 세팅
		HUD->SetViewModelChildWidget(ViewModel);

		// HUD 뷰포트에 띄우기
		HUD->AddToViewport();
	}
}
