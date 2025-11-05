// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_LockOn.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tag/AruaGameplayTags.h"
#include "Test/TestCharacterPlayer.h"
#include "Character/AbilityTask/AbilityTask_LockOn.h"

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

		// Task 시작 - 타겟 바라보기
		UAbilityTask_LockOn* LockOnTask = UAbilityTask_LockOn::CreateLockOnTask(
			this,           // OwningAbility
			CurrentTarget,  // TargetActor
			0.f          // RotationSpeed (0 = 즉시 회전)
		);

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
		CurrentTarget = nullptr;
	}

	// 부모 호출 (ActivationOwnedTags 자동 제거)
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

	// 가장 좋은 타겟 찾기 (각도 + 거리 고려)
	AActor* BestTarget = nullptr;
	float BestScore = -1.0f;

	for (AActor* Actor : FoundActors)
	{
		FVector ToTarget = (Actor->GetActorLocation() - CharacterLocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(ForwardVector, ToTarget);
		float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

		if (Angle <= LockOnAngle)
		{
			float Distance = FVector::Dist(CharacterLocation, Actor->GetActorLocation());
			float Score = (1.0f - (Angle / LockOnAngle)) * (1.0f - (Distance / LockOnRange));

			if (Score > BestScore)
			{
				BestScore = Score;
				BestTarget = Actor;
			}
		}
	}

	return BestTarget;
}