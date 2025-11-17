// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ARGC_Hit.generated.h"

/**
 * Hit 타이밍에 재생할 파티클과 사운드를 처리하는 GameplayCueNotify 클래스
 * 작성자 : 임희섭
 * 작성일 : 25/11/17
 */
UCLASS()
class ARUA_API UARGC_Hit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
    UARGC_Hit();

    virtual bool OnExecute_Implementation(
        AActor* Target,
        const FGameplayCueParameters& Parameters) const override;

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
	
};
