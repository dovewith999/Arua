// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ARCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ARCharacterPlayer.h"
#include "AbilitySystemComponent.h"

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

	StateMachineIndex = 1;
	bASCInitialized = false;
	
}

void UARCharacterAnimInstance::NativeUpdateAnimation(float DeltaTimes)
{
	Super::NativeUpdateAnimation(DeltaTimes);
	AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(Owner);

	if (!bASCInitialized)
	{
		if (!Player)
		{
			return;
		}

		UAbilitySystemComponent* PA = Player->GetAbilitySystemComponent();
		if (PA)
		{
			// 이벤트 등록 등 초기화
			bASCInitialized = true;
			PA->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("Character.Weapon"))
				.AddUObject(this, &UARCharacterAnimInstance::OnGameplayTagChanged);
		}
	}

	if (Movement)
	{
		
		Velocity = Movement->Velocity;
		WalkSpeed = Velocity.Size2D();
		bIsIdle = WalkSpeed < MovingThreshould;
		bIsWalk = Player->GetWalkState();
		bIsRun = Player->GetRunState();

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

void UARCharacterAnimInstance::OnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Character.Weapon")))
	{
		CurrentWeaponTag = Tag;
		StateMachineIndex = GetWeaponLayerIndex();
	}
}

int32 UARCharacterAnimInstance::GetWeaponLayerIndex() const
{

	AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(Owner);
	const FGameplayTagContainer& Tags = Player->GetAbilitySystemComponent()->GetOwnedGameplayTags();

	if (Tags.HasTagExact(FGameplayTag::RequestGameplayTag("Character.Weapon.None")))
		return 0;

	if (Tags.HasTagExact(FGameplayTag::RequestGameplayTag("Character.Weapon.Sword")))
		return 1;

	return 0;
}
