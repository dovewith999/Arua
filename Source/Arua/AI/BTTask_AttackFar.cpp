// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AttackFar.h"
#include "AIController.h"
#include "Enemy/Boss/ARBoss.h"

UBTTask_AttackFar::UBTTask_AttackFar()
{
	NodeName = TEXT("AttackFar");
}

EBTNodeResult::Type UBTTask_AttackFar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	AARBoss* AIPawn = Cast<AARBoss>(ControllingPawn);
	if (!AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	AIPawn->AttackShootTornado();




	return EBTNodeResult::Succeeded;


}