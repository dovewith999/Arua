// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GC/ARGC_AttackImpact.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Subsystems/ARGameEffectManager.h"

UARGC_AttackImpact::UARGC_AttackImpact()
{
}

bool UARGC_AttackImpact::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
    if (!Target)
    {
        return false;
    }

    if (!IsValid(Target))
    {
        return false;
    }

    // 사운드 재생
    if (ImpactSound)
    {
        // 위치와 방향 가져오기
        FVector Location = Parameters.Location.IsZero() ? Target->GetActorLocation() : FVector(Parameters.Location);
        FRotator Rotation = Parameters.Normal.IsZero() ? Target->GetActorRotation() : Parameters.Normal.Rotation();

        UGameplayStatics::PlaySoundAtLocation(
            Target->GetWorld(),
            ImpactSound,
            Location,
            Rotation,
            SoundVolume,
            SoundPitch,
            0.2f,  // StartTime
            nullptr,  // AttenuationSettings (기본값 사용)
            nullptr,  // ConcurrencySettings
            Target  // Owner
        );
    }

    // 파티클 재생
    if (!ImpactParticle)
    {
        return false;
    }

	const FName SocketName = TEXT("WeaponTrailEnd");

	if (ACharacter* CharacterTarget = Cast<ACharacter>(Target))
	{
		if (USkeletalMeshComponent* MeshComp = CharacterTarget->GetMesh())
		{
			// UGameplayStatics::SpawnEmitterAttached를 사용하여 파티클 재생
			UGameplayStatics::SpawnEmitterAttached(
				ImpactParticle,         // UParticleSystem* (파티클 에셋)
				MeshComp, // AttachComponent: 붙일 컴포넌트 (보통 Target 액터의 루트 컴포넌트나 SkeletalMesh)
				SocketName,             // 💡 SocketName: 파티클을 붙일 소켓 이름
				FVector::ZeroVector,    // Location: 소켓 기준 오프셋 (보통 ZeroVector)
				FRotator::ZeroRotator,  // Rotation: 소켓 기준 회전 (보통 ZeroRotator)
				ParticleScale,
				EAttachLocation::KeepRelativeOffset, // AttachmentRule: 상대적 위치 유지
				true                    // bAutoDestroy
			);
		}
	}

    if (UARGameEffectManager* GameEffectManager = GetWorld()->GetGameInstance()->GetSubsystem<UARGameEffectManager>())
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
			PlayerController->ClientStartCameraShake(GameEffectManager->GetImpactCameraShakeClass());
        }
    }

    return true;
}
