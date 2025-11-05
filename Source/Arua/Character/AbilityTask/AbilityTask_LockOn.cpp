// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilityTask_LockOn.h"

UAbilityTask_LockOn* UAbilityTask_LockOn::CreateLockOnTask(UGameplayAbility* OwningAbility, AActor* TargetActor, float RotationSpeed)
{
	UAbilityTask_LockOn* MyObj = NewAbilityTask<UAbilityTask_LockOn>(OwningAbility);

	MyObj->Target = TargetActor;
	MyObj->InterpSpeed = RotationSpeed;

	return MyObj;
}

void UAbilityTask_LockOn::Activate()
{
	Super::Activate();

	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("LockOn Task: No target!"));
		EndTask();
		return;
	}

	// Tick 활성화
	bTickingTask = true;
}

void UAbilityTask_LockOn::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

	AActor* OwnerActor = GetAvatarActor();
	if (!OwnerActor || !Target)
	{
		EndTask();
		return;
	}

	// 타겟 방향 계산
	FVector ToTarget = Target->GetActorLocation() - OwnerActor->GetActorLocation();
	ToTarget.Z = 0.0f; // 수평 회전만 (Yaw)

	if (ToTarget.IsNearlyZero())
		return;

	FRotator TargetRotation = ToTarget.Rotation();
	FRotator CurrentRotation = OwnerActor->GetActorRotation();

	// 회전 보간
	FRotator NewRotation;
	if (InterpSpeed > 0.0f)
	{
		// 부드러운 회전
		NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);
	}
	else
	{
		// 즉시 회전
		NewRotation = TargetRotation;
	}

	// Pitch와 Roll은 유지, Yaw만 변경
	NewRotation.Pitch = CurrentRotation.Pitch;
	NewRotation.Roll = CurrentRotation.Roll;

	OwnerActor->SetActorRotation(NewRotation);
}