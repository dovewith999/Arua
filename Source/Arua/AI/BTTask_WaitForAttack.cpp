// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WaitForAttack.h"
#include "AI/ARAI.h"

UBTTask_WaitForAttack::UBTTask_WaitForAttack()
{
	NodeName = TEXT("WaitForAttack");
}

EBTNodeResult::Type UBTTask_WaitForAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float NewWaitTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_WAITTIME);
	SetWaitTime(NewWaitTime);

	return EBTNodeResult::Succeeded;
}
