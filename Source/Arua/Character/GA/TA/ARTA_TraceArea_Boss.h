// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARTA_TraceBase.h"
#include "ARTA_TraceArea_Boss.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARTA_TraceArea_Boss : public AARTA_TraceBase
{
	GENERATED_BODY()

public:
	AARTA_TraceArea_Boss();

	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;


protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
	
};
