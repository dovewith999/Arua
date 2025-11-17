// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTA_TraceWideRange.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ARCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

AARTA_TraceWideRange::AARTA_TraceWideRange()
{
}

void AARTA_TraceWideRange::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AARTA_TraceWideRange::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AARTA_TraceWideRange::MakeTargetData() const
{
	Super::MakeTargetData();
	
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	// 공격 범위 설정: 박스 크기
	const FVector AttackRadius = FVector(500.0f, 500.0f, 500.0f);	
	// 트레이스 시작 위치는 플레이어의 위치 앞부분
	const FVector PlayerStartLocation = Character->GetActorLocation() + (Character->GetActorForwardVector() * (AttackRadius.X));

	const FQuat PlayerRotation = Character->GetActorRotation().Quaternion();

	// 오버랩 결과를 담을 배열
	TArray<FOverlapResult> OverlapResults;
	// 충돌 쿼리 설정 (자신 무시)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UARTA_Trace), false, Character);

	// 충돌 모양: X, Y, Z 반경을 가지는 박스 형태
	FCollisionShape BoxShape = FCollisionShape::MakeBox(AttackRadius);

	// OverlapMultiByChannel을 사용하여 구형 범위 내의 모든 대상 감지
	bool HitDetected = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		PlayerStartLocation,
		/*FQuat::Identity,*/ // 회전 없음
		PlayerRotation,
		CCHANNEL_ARACTION, // 사용할 충돌 채널
		BoxShape,
		Params
	);

	FGameplayAbilityTargetDataHandle DataHandle;

	if (HitDetected)
	{
		// 오버랩된 모든 대상을 타겟 데이터로 변환
		for (const FOverlapResult& Overlap : OverlapResults)
		{
			// FGameplayAbilityTargetData_ActorArray를 사용하여 여러 액터를 담을 수도 있지만,
			// 여기서는 각 오버랩 결과를 FGameplayAbilityTargetData_SingleTargetHit으로 변환하여 핸들에 추가
			FHitResult DummyHitResult;
			DummyHitResult.HitObjectHandle = Overlap.OverlapObjectHandle; // 오버랩된 객체 핸들

			// SingleTargetHit 대신 ActorArray를 사용하는 것이 더 깔끔할 수 있음
			// 여기서는 기존의 HitResult 구조를 따르기 위해 SingleTargetHit을 사용.
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(DummyHitResult);
			DataHandle.Add(TargetData);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
		// 플레이어 위치에 반경 AttackRadius의 박스를 그림.
		DrawDebugBox(GetWorld(), PlayerStartLocation, AttackRadius, PlayerRotation, DrawColor, false, 3.0f);
	}
#endif

	return DataHandle;
}
