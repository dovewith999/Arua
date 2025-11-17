// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC/ARGC_Attack.h"
#include "Kismet/GameplayStatics.h"

UARGC_Attack::UARGC_Attack()
{
	SoundVolume = 1.0f;
	SoundPitch = 1.0f;
}

bool UARGC_Attack::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
    // 사운드 재생
    if (HitSound)
    {
        // 위치와 방향 가져오기
        FVector Location = Parameters.Location.IsZero() ? Target->GetActorLocation() : FVector(Parameters.Location);

        FRotator Rotation = Parameters.Normal.IsZero() ? Target->GetActorRotation() : Parameters.Normal.Rotation();

        UGameplayStatics::PlaySoundAtLocation(
            Target->GetWorld(),
            HitSound,
            Location,
            Rotation,
            SoundVolume,
            SoundPitch,
            0.0f,  // StartTime
            nullptr,  // AttenuationSettings (기본값 사용)
            nullptr,  // ConcurrencySettings
            Target  // Owner
        );
    }

    return true;
}