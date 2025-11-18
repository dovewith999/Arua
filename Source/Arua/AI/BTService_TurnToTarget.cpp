// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_TurnToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "AI/ARAI.h"
#include "Controller/AruaPlayerController.h"
#include "Enemy/Boss/ARBoss.h"


UBTService_TurnToTarget::UBTService_TurnToTarget()
{
	NodeName = TEXT("TurnToTarget");

    Interval = 0.1f;
}

void UBTService_TurnToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYERACTOR));
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    AAIController* AIC = OwnerComp.GetAIOwner();
    AARBoss* AIPawn = Cast<AARBoss>(ControllingPawn);

    FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
    FRotator CurrentRot = ControllingPawn->GetActorRotation();

    FRotator FinalRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, AIPawn->GetBossTurnSpeed());
    ControllingPawn->SetActorRotation(FinalRot);


}
