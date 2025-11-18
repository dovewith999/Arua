// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GA/TA/ARTA_Trace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ARCollision.h"
#include "DrawDebugHelpers.h"

AARTA_Trace::AARTA_Trace()
{
}

void AARTA_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AARTA_Trace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AARTA_Trace::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FHitResult OutHitResult;
	const float AttackRange = 100.0f;
	const float AttackRadius = 50.f;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(UARTA_Trace), false, Character);
	const FVector Forward = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + Forward * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_ARACTION, FCollisionShape::MakeSphere(AttackRadius),Params);

	FGameplayAbilityTargetDataHandle DataHandle;
	if (HitDetected)
	{
		FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(OutHitResult);
		DataHandle.Add(TargetData);
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);

		if (HitDetected)
		{
			// 타격이 감지되었을 경우, 실제 충돌 지점(ImpactPoint)을 노란색 점으로 표시
			DrawDebugPoint(GetWorld(), OutHitResult.ImpactPoint, 15.0f, FColor::Yellow, false, 5.0f);

			// 추가: 충돌 표면의 법선 벡터(ImpactNormal) 시각화 (타격 방향 확인용)
			DrawDebugLine(GetWorld(), OutHitResult.ImpactPoint, OutHitResult.ImpactPoint + OutHitResult.ImpactNormal * 50.0f, FColor::Cyan, false, 5.0f, 0, 3.0f);
		}
	}
#endif

	return DataHandle;	
}
