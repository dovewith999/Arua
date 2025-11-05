// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ARAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AARAIController();

	virtual void OnPossess(APawn* InPawn) override;
	void RunAI();
	void StopAI();

private:

	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;

	// BT ¾Ö¼Â.
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;

};
