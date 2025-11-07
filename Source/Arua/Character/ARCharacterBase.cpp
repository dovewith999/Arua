// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AARCharacterBase::AARCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// 상속받은 클래스의 생성자에서 초기화.
	ASC = nullptr;
	AttributeSet = nullptr;
}

UAbilitySystemComponent* AARCharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AARCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AARCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(
		EMovementMode::MOVE_None
	);

	// 죽는 애니메이션 재생. 
	// PlayDeadAnimation();

	// 콜리전 끄기.
	SetActorEnableCollision(false);
}

void AARCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance
		= GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);

		const float PlayRate = 1.0f;
		AnimInstance->Montage_Play(
			DeadMontage,
			PlayRate
		);

	}
}

