// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/ARAI.h"
#include "Controller/AruaPlayerController.h"
#include "Enemy/ARMonsterBase.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{

	NodeName = TEXT("DetectPlayer");

	Interval = 0.1f;
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AAIController* AIC = OwnerComp.GetAIOwner();
	APawn* AIPawn = AIC->GetPawn();

	if (BB)
	{
		AAruaPlayerController* PC = Cast<AAruaPlayerController>(UGameplayStatics::GetPlayerController(AIPawn, 0));
		APawn* PlayerPawn = PC->GetPawn();
		if (PlayerPawn)
		{
			//if (PlayerActorKey.SelectedKeyType)
			BB->SetValueAsObject(BBKEY_PLAYERACTOR, PlayerPawn);

			//if (PlayerPosKey.SelectedKeyType)
			BB->SetValueAsVector(BBKEY_PLAYERPOS, PlayerPawn->GetActorLocation());
		}
	}
}
