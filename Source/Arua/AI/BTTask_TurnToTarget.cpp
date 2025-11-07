// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ARAI.h"
#include "Enemy/Boss/ARBoss.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("TurntoTarget");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// NPC가 캐릭터를 바라보는 방향.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	// 캐릭터 구하기.
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYERACTOR));

	if (!TargetPawn)
	{
		return EBTNodeResult::Failed;
	}

	AARBoss* AIPawn = Cast<AARBoss>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	// NPC가 바라볼 방향 구하기.
	FVector LookVector = TargetPawn->GetActorLocation() - ControllingPawn->GetActorLocation();

	// 회전 값 구하기.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	// DeltaTime.
	UWorld* World = ControllingPawn->GetWorld();

	//회전 속도
	float TurnSpeed = AIPawn->GetBossTurnSpeed();

	//부드럽게 회전 처리
	FRotator FinalRotator = FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, World->GetDeltaSeconds(), TurnSpeed);

	//회전 설정
	ControllingPawn->SetActorRotation(FinalRotator);

	return EBTNodeResult::Succeeded;



}
