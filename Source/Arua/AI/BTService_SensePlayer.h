// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SensePlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UBTService_SensePlayer : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_SensePlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 감지 반경 
	UPROPERTY(EditAnywhere, Category = "Sense")
	float SenseRadius = 50.0f * 30 * 2;

};
