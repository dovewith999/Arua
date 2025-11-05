// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_LockOn.generated.h"

/**
 * LockOn 기능을 어빌리티화 한 클래스입니다.
 * 해당 기능 활성화 시 Owner에게 Player_State_LockOn을 부여합니다.
 * 이는 추후 다른 액터에도 필요하면 수정하겠습니다.
 * 작성자 : 임희섭
 * 작성일 : 25/11/04
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