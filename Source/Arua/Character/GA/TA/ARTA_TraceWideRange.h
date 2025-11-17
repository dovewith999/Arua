// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GA/TA/ARTA_TraceBase.h"
#include "ARTA_TraceWideRange.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARTA_TraceWideRange : public AARTA_TraceBase
{
	GENERATED_BODY()

public:
	AARTA_TraceWideRange();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;


protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
	
};
