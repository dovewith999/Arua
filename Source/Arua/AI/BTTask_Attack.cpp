// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "Enemy/Boss/ARBoss.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
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

	//랜덤하게 공격하도록 로직 생성

	int32 RandAttackNum = FMath::RandRange(0, 2);

	switch (RandAttackNum)
	{
	case 0: //브레스 공격
		AIPawn->AttackFireBreathSwipe();
		break;


	case 1: //왼발 공격

		AIPawn->AttackPawLeft();
		break;

	case 2: //콤보 1 공격

		AIPawn->ComboAttackPawLeft_TailRight();
		break;
	}

	return EBTNodeResult::Succeeded;


}
