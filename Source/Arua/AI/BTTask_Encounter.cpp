// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Encounter.h"
#include "AIController.h"
#include "Enemy/Boss/ARBoss.h"

UBTTask_Encounter::UBTTask_Encounter()
{
	NodeName = TEXT("Encounter");
}

EBTNodeResult::Type UBTTask_Encounter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	AARBoss* AIPawn = Cast<AARBoss>(ControllingPawn);


	AIPawn->BossEncounter();

	return EBTNodeResult::Succeeded;
}
