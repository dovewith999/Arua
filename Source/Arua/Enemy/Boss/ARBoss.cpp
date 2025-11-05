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


	static ConstructorHelpers::FObjectFinder<UAnimMontage> Montage_Attack_FireBreath_SwipeRef(TEXT("/Game/Personal/LEE_J_S/Animation/AM_Attack_FireBreath_Swipe.AM_Attack_FireBreath_Swipe_C"));
	if (Montage_Attack_FireBreath_SwipeRef.Succeeded())
	{
		Montage_Attack_FireBreath_Swipe = Montage_Attack_FireBreath_SwipeRef.Object;
	}

	AIControllerClass = AARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



}

void AARBoss::AttackFireBreathSwipe()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(Montage_Attack_FireBreath_Swipe);

	}
}