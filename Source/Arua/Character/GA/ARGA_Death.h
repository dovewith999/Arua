// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ARGA_Death.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGA_Death : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UARGA_Death();

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
    // 몽타주가 끝나면 호출될 함수 - 25/11/07 임희섭
    UFUNCTION()
    void OnDeathCallback();

    // 이 어빌리티를 실행하는 Avator를 캐싱할 변수;
    UPROPERTY()
    TObjectPtr<class AARCharacterBase> Avator;
	
};
