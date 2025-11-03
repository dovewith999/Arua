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
		bIsRun = Cast<AARCharacterPlayer>(Owner)->GetRunState();
	}

}
