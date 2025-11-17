// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ARGC_Attack.generated.h"

/**
 * 플레이어가 공격을 시도할 때 재생할 효과를 처리하는 GameplayCueNotify 클래스
 * 작성자 : 임희섭
 * 작성일 : 25/11/17
 */
UCLASS()
class ARUA_API UARGC_Attack : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
    UARGC_Attack();

    virtual bool OnExecute_Implementation(
        AActor* Target,
        const FGameplayCueParameters& Parameters) const override;

private:
    // 재생할 사운드
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    TObjectPtr<class USoundBase> HitSound;

    // 사운드 볼륨
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float SoundVolume;

    // 사운드 피치
    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    float SoundPitch;
	
};
