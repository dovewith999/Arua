// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AruaPlayerController.h"
#include "UI/View/PlayerHUDView.h"
#include "UI/Model/PlayerData.h"
#include "UI/ViewModel/PlayerViewModel.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "Enemy/ARMonsterBase.h"
#include "UI/ViewModel/BossViewModel.h"
#include "Tag/AruaGameplayTags.h"
#include "Character/ARCharacterPlayer.h"

#include "UI/Quest/QuestHUDWidget.h"
#include "UI/Quest/QuestJournalWidget.h"
#include "Components/Quest/QuestComponent.h"

AAruaPlayerController::AAruaPlayerController()
{
	//static ConstructorHelpers::FClassFinder<UPlayerHUDView> PlayerHUDRef(TEXT("/Game/Personal/LIM_H_S/UI/WBP_HUD.WBP_HUD_C"));

	//if (PlayerHUDRef.Succeeded())
	//{
	//	HUDClass = PlayerHUDRef.Class;
	//}

	//static ConstructorHelpers::FClassFinder<UQuestHUDWidget> QuestHUDWidgetRef(TEXT("/Game/Blueprints/Widgets/Player/WBP_QuestHUD.WBP_QuestHUD"));
	//if (QuestHUDWidgetRef.Succeeded())
	//{
	//	QuestHUDClass = QuestHUDWidgetRef.Class;
	//}
}

void AAruaPlayerController::SetTargetBoss(AARMonsterBase* InTargetMonster)
{
	HUD->SetBossViewModel(InTargetMonster->GetViewModel());
}

void AAruaPlayerController::TargetOff()
{
	HUD->TurnOffBossHpBar();
	if (AARCharacterPlayer* OwnerPlayer = Cast<AARCharacterPlayer>(GetPawn()))
	{
		UE_LOG(LogTemp, Log, TEXT("OwnerPlayer is valid"));

		OwnerPlayer->FinishLockOn();
	}
}

void AAruaPlayerController::CanInteract(bool InCanInteract, FName InObjectName)
{
	HUD->SetCanInteract(InCanInteract, InObjectName);	
}

void AAruaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly Mode;
	this->SetInputMode(Mode);

	// 퀘스트 HUD 위젯 생성 및 그리기
	if (QuestHUDClass)
	{
		QuestHUD = CreateWidget<UQuestHUDWidget>(this, QuestHUDClass);
		if (QuestHUD)
		{
			QuestHUD->AddToViewport();
		}
	}

	// 퀘스트 저널 위젯 생성 및 그리기
	if (QuestJournalClass)
	{
		QuestJournal = CreateWidget<UQuestJournalWidget>(this, QuestJournalClass);
		if (QuestJournal)
		{
			QuestJournal->AddToViewport();
			QuestJournal->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (UQuestComponent* QuestComponent = GetPawn()->FindComponentByClass<UQuestComponent>())
	{
		// HUD/저널 위젯과 플레이어의 퀘스트 컴포넌트 바인딩
		if (QuestHUD)
		{
			QuestHUD->BindToQuestComponent(QuestComponent);
		}
		if (QuestJournal)
		{
			//QuestJournal->BindToQuestComponent(QuestComponent);
		}
	}
}

void AAruaPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (HUD = Cast<UPlayerHUDView>(CreateWidget(this, HUDClass)))
	{
		UPlayerData* PlayerData = NewObject<UPlayerData>();

		if (AARCharacterPlayer* OwnerPlayer = Cast<AARCharacterPlayer>(aPawn))
		{
			UPlayerAttributeSet* PlayerAttributeSet = Cast<UPlayerAttributeSet>(OwnerPlayer->GetAttributeSet());
			PlayerData->BindToAttributeSet(PlayerAttributeSet);
		}

		ViewModel = NewObject<UPlayerViewModel>();
		ViewModel->Initialize(PlayerData);

		// 뷰모델 세팅
		HUD->SetViewModelChildWidget(ViewModel);

		// HUD 뷰포트에 띄우기
		HUD->AddToViewport();

		// Todo : 연동 확인용 값 초기화
		ViewModel->SetMaxHp(100.f);
		ViewModel->SetHp(100.f);
	}
}
