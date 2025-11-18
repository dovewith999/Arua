// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TurnToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UBTService_TurnToTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_TurnToTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
