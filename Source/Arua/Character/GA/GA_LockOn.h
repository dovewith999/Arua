// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_LockOn.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UGA_LockOn : public UGameplayAbility
{
    GENERATED_BODY()

public:
    UGA_LockOn();

protected:
    virtual void ActivateAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData
    ) override;

    virtual void EndAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateEndAbility,
        bool bWasCancelled
    ) override;

private:
    // Lock-On 타겟 찾기
    AActor* FindLockOnTarget();

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
    float LockOnRange = 1500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
    float LockOnAngle = 45.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
    TSubclassOf<AActor> TargetActorClass;

    // 현재 Lock-On 중인 타겟
    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget;
};