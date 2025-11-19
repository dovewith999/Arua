// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ARGC_SpecialAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGC_SpecialAttack : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
    UARGC_SpecialAttack();

public:
    virtual bool OnExecute_Implementation(
        AActor* Target,
        const FGameplayCueParameters& Parameters) const override;

    // 타이머를 설정하고 시작하는 함수
    void StartOffsetEffect();

    // 타이머에 의해 반복 호출되어 파라미터를 업데이트하는 함수
    void UpdateOffsetEffect();

    // 효과가 완료되었을 때 호출되는 정리 함수
    void FinishOffsetEffect();

protected:
    // 재생할 나이아가라 시스템
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<class UNiagaraSystem> HitParticle;

    // 재생할 사운드
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<class USoundBase> HitSound;

    // 파티클 스케일
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    FVector ParticleScale;

    // 사운드 볼륨
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float SoundVolume;

    // 사운드 피치
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float SoundPitch;

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
