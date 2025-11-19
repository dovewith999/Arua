// Fill out your copyright notice in the Description page of Project Settings.


#include "ARTA_TraceArea.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Physics/ARCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

AARTA_TraceArea::AARTA_TraceArea()
{
}

void AARTA_TraceArea::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AARTA_TraceArea::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		FGameplayAbilityTargetDataHandle DataHandle = MakeTargetData();
		TargetDataReadyDelegate.Broadcast(DataHandle);
	}
}

FGameplayAbilityTargetDataHandle AARTA_TraceArea::MakeTargetData() const
{
	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	// 공격 범위 설정: 플레이어 주변의 반지름
	const float AttackRadius = 300.0f;
	// 트레이스 시작 위치는 플레이어의 위치
	const FVector PlayerStartLocation = Character->GetActorLocation();

	// 오버랩 결과를 담을 배열
	TArray<FOverlapResult> OverlapResults;
	// 충돌 쿼리 설정 (자신 무시)
	FCollisionQueryParams Params(SCENE_QUERY_STAT(UARTA_Trace), false, Character);

	// 충돌 모양: 반지름 AttackRadius인 구체
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(AttackRadius);

	// OverlapMultiByChannel을 사용하여 구형 범위 내의 모든 대상 감지
	bool HitDetected = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		PlayerStartLocation,
		FQuat::Identity, // 회전 없음
		CCHANNEL_ARACTION, // 사용할 충돌 채널
		SphereShape,
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
			//DummyHitResult.SetActor() = Overlap.GetActor(); // 오버랩된 액터

			// SingleTargetHit 대신 ActorArray를 사용하는 것이 더 깔끔할 수 있음
			// 여기서는 기존의 HitResult 구조를 따르기 위해 SingleTargetHit을 사용.
			FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(DummyHitResult);
			DataHandle.Add(TargetData);
		}
	}

#if ENABLE_DRAW_DEBUG
	//if (bShowDebug)
	//{
	//	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
	//	// 플레이어 위치에 반경 AttackRadius의 구체를 그림.
	//	DrawDebugSphere(GetWorld(), PlayerStartLocation, AttackRadius, 16, DrawColor, false, 5.0f);
	//}
#endif

	return DataHandle;
}