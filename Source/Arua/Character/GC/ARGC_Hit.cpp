// Fill out your copyright notice in the Description page of Project Settings.

#include "ARGC_Hit.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

UARGC_Hit::UARGC_Hit()
{
	ParticleScale = FVector(1.0f, 1.0f, 1.0f);
	SoundVolume = 1.0f;
	SoundPitch = 1.0f;
}

bool UARGC_Hit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
    if (!Target)
    {
        return false;
    }

    // 위치와 방향 가져오기
    //FVector Location = Parameters.Location.IsZero() ? Target->GetActorLocation() : FVector(Parameters.Location);
    FVector Location = Parameters.Location.IsZero() ? Cast<AActor>(Parameters.SourceObject)->GetActorLocation() : FVector(Parameters.Location);
    FRotator Rotation = Parameters.Normal.IsZero() ? Target->GetActorRotation() : Parameters.Normal.Rotation();

    // 나이아가라 파티클 재생
    if (HitParticle)
    {
        UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            Target->GetWorld(),
            HitParticle,
            Location,
            Rotation,
            ParticleScale,
            true,  // bAutoDestroy
            true,  // bAutoActivate
            ENCPoolMethod::AutoRelease,
            true   // bPreCullCheck
        );

        // 추가 파라미터 설정 가능
        if (NiagaraComp)
        {
            // 예: 대미지 값에 따른 크기 조절
            if (Parameters.RawMagnitude > 0.0f)
            {
                float ScaleMultiplier = FMath::Clamp(
                    Parameters.RawMagnitude / 100.0f,
                    0.5f,
                    2.0f
                );
                NiagaraComp->SetWorldScale3D(ParticleScale * ScaleMultiplier);
            }
        }
    }

    // 사운드 재생
    if (HitSound)
    {
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
