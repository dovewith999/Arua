// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilityTask_LockOn.h"
#include "Test/TestCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"

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

	// Todo : 값을 가져와야 함
	LockOnCameraOffset = FVector(-200.f, 0.f, 100.f);
}

void UAbilityTask_LockOn::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (!ShouldBroadcastAbilityTaskDelegates())
	{
		return;
	}

#pragma region 플레이어 액터 회전

	AActor* OwnerActor = GetAvatarActor();
	if (!OwnerActor || !Target)
	{
		EndTask();
		return;
	}

	// 타겟 방향 계산
	FVector ToTarget = Target->GetActorLocation() - OwnerActor->GetActorLocation();
	ToTarget.Z = 0.0f; // 수평 회전만 (Yaw)

	// 거리가 멀어지면 Task 종료 및 LockOn 상태 비활성화
	// Todo : 거리 수치는 나중에 데이터 테이블 작성해서 받아오도록 하기
	if (ToTarget.Length() > 1500.0f)
	{
		OnLostTarget.Broadcast();
		EndTask();
		return;
	}

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
#pragma endregion

#pragma region 카메라 회전
	if (ATestCharacterPlayer* Player = Cast<ATestCharacterPlayer>(OwnerActor))
	{
		if (USpringArmComponent* SpringArm = Player->GetSpringArm())
		{
			SpringArm->SocketOffset = LockOnCameraOffset;
			SpringArm->bUsePawnControlRotation = false;
		}
	}
#pragma endregion
}