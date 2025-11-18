// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GA/TA/ARTA_TraceBase.h"
#include "ARTA_Trace_Boss.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARTA_Trace_Boss : public AARTA_TraceBase
{
	GENERATED_BODY()
	
public:
	AARTA_Trace_Boss();
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;

};
