// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_LockOn.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/AruaGameplayTags.h"
#include "Test/TestCharacterPlayer.h"
#include "Character/AbilityTask/AbilityTask_LockOn.h"
#include "Enemy/ARMonsterBase.h"

UGA_LockOn::UGA_LockOn()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// 어빌리티 식별 Tag
	AbilityTags.AddTag(AruaGamePlayTags::Ability_LockOn);

	// 활성화 시 Owner에게 부여되는 Tag
	ActivationOwnedTags.AddTag(AruaGamePlayTags::Player_State_LockOn);
}

void UGA_LockOn::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//// 타겟 찾기
	AActor* Target = FindLockOnTarget();

	if (Target)
	{
		CurrentTarget = Target;
		// FName을 FString으로 변환하고 포맷팅
		FString DebugMessage = FString::Printf(
			TEXT("Lock-On Started : %s"),
			*(Target->GetName())
		);

		// 화면 출력
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Green,
			DebugMessage
		);

		if (AARMonsterBase* TargetMonster = Cast<AARMonsterBase>(CurrentTarget))
		{
			TargetMonster->SetTargetLockWidget(false);
		}

		// Task 시작 - 타겟 바라보기
		UAbilityTask_LockOn* LockOnTask = UAbilityTask_LockOn::CreateLockOnTask(
			this,           // OwningAbility
			CurrentTarget,  // TargetActor
			10.f          // RotationSpeed (0 = 즉시 회전)
		);

		LockOnTask->OnLostTarget.AddDynamic(this, &UGA_LockOn::LostTarget);

		LockOnTask->ReadyForActivation();
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Target Not Found"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UGA_LockOn::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	// 정리 작업
	if (CurrentTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("Lock-On Ended: %s"), *CurrentTarget->GetName());

		if (AARMonsterBase* TargetMonster = Cast<AARMonsterBase>(CurrentTarget))
		{
			TargetMonster->SetTargetLockWidget(true);
		}

		CurrentTarget = nullptr;
	}

	// 부모 호출 (ActivationOwnedTags 자동 제거)
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		return;
	}

	if (ATestCharacterPlayer* Owner = Cast<ATestCharacterPlayer>(Character))
	{
		Owner->FinishLockOn();
	}
}

AActor* UGA_LockOn::FindLockOnTarget()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		return nullptr;
	}

	FVector CharacterLocation = Character->GetActorLocation();
	FVector ForwardVector = Character->GetActorForwardVector();

	// 주변 타겟 탐색
	TArray<AActor*> FoundActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		CharacterLocation,
		LockOnRange,
		ObjectTypes,
		TargetActorClass,
		ActorsToIgnore,
		FoundActors
	);

	// 디버그 구체 그리기
	DrawDebugSphere(
		GetWorld(),             // 월드 객체 포인터
		CharacterLocation,      // 구체의 중심 위치 (OverlapActors의 첫 번째 인자와 동일)
		LockOnRange,            // 구체의 반지름 (OverlapActors의 세 번째 인자와 동일)
		12,                     // 구체를 구성하는 선분의 개수 (높을수록 부드러움)
		FColor::Red,            // 구체의 색상 (원하는 색상으로 설정)
		false,                  // bPersistentLines: true면 계속 남아있고, false면 Duration 동안만 표시
		3.0f                    // Duration: 구체가 화면에 표시될 시간 (초 단위)
	);

	// 가장 좋은 타겟 찾기 (각도 + 거리 고려)
	AActor* BestTarget = nullptr;
	float BestScore = -1.0f;

	for (AActor* Actor : FoundActors)
	{
		if (BestTarget == nullptr)
		{
			BestScore = FVector::Dist(CharacterLocation, Actor->GetActorLocation());
			BestTarget = Actor;
			continue;
		}

		float Distance = FVector::Dist(CharacterLocation, Actor->GetActorLocation());
		if (BestScore > Distance)
		{
			BestScore = Distance;
			BestTarget = Actor;
			continue;
		}

		//FVector ToTarget = (Actor->GetActorLocation() - CharacterLocation).GetSafeNormal();
		//float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);
		//float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

		//if (Angle <= LockOnAngle)
		//{
		//	float Distance = FVector::Dist(CharacterLocation, Actor->GetActorLocation());
		//	float Score = (1.0f - (Angle / LockOnAngle)) * (1.0f - (Distance / LockOnRange));

		//	if (Score > BestScore)
		//	{
		//		BestScore = Score;
		//		BestTarget = Actor;
		//	}
		//}
	}

	return BestTarget;
}

void UGA_LockOn::LostTarget()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Lost Target"));
	K2_EndAbility();
}
