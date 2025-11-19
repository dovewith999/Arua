// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Encounter.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UBTTask_Encounter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Encounter();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
