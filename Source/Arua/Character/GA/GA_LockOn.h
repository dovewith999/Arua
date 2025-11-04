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

    virtual void InputReleased(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo
    ) override;

    virtual void CancelAbility(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        bool bReplicateCancelAbility
    ) override;

private:
    // Lock-On 타겟 찾기
    AActor* FindLockOnTarget();

    // Lock-On 시작
    void StartLockOn(AActor* Target);

    // Lock-On 종료
    void EndLockOn();

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
    float LockOnRange = 1500.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
    float LockOnAngle = 45.0f;

    UPROPERTY(EditDefaultsOnly, Category = "LockOn")
    TSubclassOf<AActor> TargetActorClass;

    // 현재 Lock-On 중인 타겟
    UPROPERTY()
    TObjectPtr<AActor> CurrentTarget;

    // Lock-On 상태를 나타내는 GameplayTag (선택사항)
    FGameplayTag LockOnStateTag;

	
};
