// Fill out your copyright notice in the Description page of Project Settings.


#include "ARDamageExecutionCalculation.h"
#include "AbilitySystemComponent.h"

UARDamageExecutionCalculation::UARDamageExecutionCalculation()
{
}

void UARDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent(); // Å¸°ÙÀÇ ASC
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent(); // OwenerÀÇ ASC

    AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
}
