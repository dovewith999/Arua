// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ARCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ARCharacterPlayer.h"

UARCharacterAnimInstance::UARCharacterAnimInstance()
{
	MovingThreshould = 3.0f;
}

void UARCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UARCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimes)
{
	Super::NativeUpdateAnimation(DeltaTimes);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		WalkSpeed = Velocity.Size2D();
		bIsIdle = WalkSpeed < MovingThreshould;
		bIsWalk = Cast<AARCharacterPlayer>(Owner)->GetWalkState();
		bIsRun = Cast<AARCharacterPlayer>(Owner)->GetRunState();
		if (Cast<AARCharacterPlayer>(Owner)->GetIsWeaponChanged())
		{
			WeaponType = Cast<AARCharacterPlayer>(Owner)->GetWeapon()->GetWeaponType();
			Cast<AARCharacterPlayer>(Owner)->SetIsWeaponChanged(false);
			bIsWeaponChanged = true;
		}

		// 이동 방향
		FVector VelocityDirection = Velocity.GetSafeNormal();

		// 캐릭터의 Forward 벡터
		FVector Forward = Owner->GetActorForwardVector();

		// 각도 계산 (-180 ~ 180)
		Angle = FMath::RadiansToDegrees(FMath::Atan2(
			FVector::CrossProduct(Forward, VelocityDirection).Z,
			FVector::DotProduct(Forward, VelocityDirection)
		));
	}
}
