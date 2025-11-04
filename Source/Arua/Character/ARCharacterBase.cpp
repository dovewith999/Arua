// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AARCharacterBase::AARCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

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
	PlayDeadAnimation();

	// 콜리전 끄기.
	SetActorEnableCollision(false);
}

void AARCharacterBase::PlayDeadAnimation()
{
	// 몽타주 재생.
	UAnimInstance* AnimInstance
		= GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 이미 재생 중인 몽타주가 있을 수 있으니 모두 종료.
		AnimInstance->StopAllMontages(0.0f);

		// 재생 속도.
		/* 나중에 DeadMontage 생성자에서 초기화하고 사용하시면 될 것 같습니다.
		const float PlayRate = 1.0f;
		AnimInstance->Montage_Play(
			DeadMontage,
			PlayRate
		);
		*/
	}
}

