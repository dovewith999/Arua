// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GA/ARGA_PlayerAbility.h"
#include "ARGA_PlayerChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGA_PlayerChargeAttack : public UARGA_PlayerAbility
{
	GENERATED_BODY()
public:
    UARGA_PlayerChargeAttack();
	
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

    virtual void InputReleased(
        const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
    UFUNCTION()
    void OnCompleteCallback();

    UFUNCTION()
    void OnInterruptedCallback();

    // 타이머를 설정하고 시작하는 함수
    void StartOffsetEffect();

    // 타이머에 의해 반복 호출되어 파라미터를 업데이트하는 함수
    void UpdateOffsetEffect();

    // 효과가 완료되었을 때 호출되는 정리 함수
    void FinishOffsetEffect();

protected:
    UPROPERTY()
    bool bInputReleaseHandled = false;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> ChargeAttackMontage;

#pragma region UVOffsetEffect
    UPROPERTY(EditDefaultsOnly, Category = "Post Process")
    float OffsetDuration = 0.3f; // 0.0 -> 0.5 -> 0.0 까지 걸리는 총 시간

    UPROPERTY()
    UMaterialInstanceDynamic* UVOffsetMID; // Post Process에 할당된 MID 참조

    // 타이머 관련 변수
    FTimerHandle OffsetTimerHandle; // 타이머 핸들
    float CurrentOffsetTime = 0.0f; // 현재 진행 시간
#pragma endregion
};
