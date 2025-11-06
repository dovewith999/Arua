// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/ARBoss.h"
#include "AI/ARAIController.h"

AARBoss::AARBoss()
{
	// 메시 컴포넌트 설정.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f)
	);

	// 애셋 지정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/Elemental_Dragon/Meshes/SK_Elemental_Dragon_Toon.SK_Elemental_Dragon_Toon"));

	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}


	// 브레스 몽타주 에셋 지정

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAttackFireBreathSwipeRef(TEXT("/Game/Personal/LEE_J_S/Animation/AM_Attack_FireBreath_Swipe.AM_Attack_FireBreath_Swipe"));
	if (MontageAttackFireBreathSwipeRef.Succeeded())
	{
		MontageAttackFireBreathSwipe = MontageAttackFireBreathSwipeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAttackPawLeftRef(TEXT("/Game/Personal/LEE_J_S/Animation/AM_Attack_Paw_Left.AM_Attack_Paw_Left"));
	if (MontageAttackPawLeftRef.Succeeded())
	{
		MontageAttackPawLeft = MontageAttackPawLeftRef.Object;
	}


	AIControllerClass = AARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



}

void AARBoss::AttackFireBreathSwipe()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackFireBreathSwipe);

	}
}

void AARBoss::AttackPawLeft()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackPawLeft);

	}
}