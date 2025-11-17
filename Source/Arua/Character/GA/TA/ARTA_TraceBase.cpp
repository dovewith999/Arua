// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/TA/ARTA_TraceBase.h"

void AARTA_TraceBase::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}

void AARTA_TraceBase::ConfirmTargetingAndContinue()
{
	Super::ConfirmTargeting();
}

FGameplayAbilityTargetDataHandle AARTA_TraceBase:: MakeTargetData() const
{
	return FGameplayAbilityTargetDataHandle();
}