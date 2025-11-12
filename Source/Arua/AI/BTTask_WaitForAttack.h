// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTask_WaitForAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UBTTask_WaitForAttack : public UBTTask_Wait
{
	GENERATED_BODY()

public:

	UBTTask_WaitForAttack();

	FORCEINLINE void SetWaitTime(float AttackTime) { WaitTime = AttackTime; }
	

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
