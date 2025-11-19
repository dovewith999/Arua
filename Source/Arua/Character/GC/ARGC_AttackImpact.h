// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ARGC_AttackImpact.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGC_AttackImpact : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
    UARGC_AttackImpact();

    virtual bool OnExecute_Implementation(
        AActor* Target,
        const FGameplayCueParameters& Parameters) const override;

protected:
    // 재생할 파티클 시스템
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<class UParticleSystem> ImpactParticle;

    // 재생할 사운드
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<class USoundBase> ImpactSound;

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
