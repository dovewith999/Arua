// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GA/ARGA_PlayerAbility.h"
#include "ARGA_PlayerAddChargeCount.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGA_PlayerAddChargeCount : public UARGA_PlayerAbility
{
	GENERATED_BODY()

public:
    UARGA_PlayerAddChargeCount();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    virtual void CancelAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateCancelAbility) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;
	
private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UGameplayEffect> AddChargeEffectClass;
};
