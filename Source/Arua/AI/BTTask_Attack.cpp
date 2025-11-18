// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "Enemy/Boss/ARBoss.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("AttackNear");

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	int32 RandomNum = FMath::RandRange(1, 1);

	if (RandomNum == 0)
		AIPawn->AttackPawLeft();
	else
		AIPawn->ComboAttackPawLeft_TailRight();


	return EBTNodeResult::Succeeded;


}
