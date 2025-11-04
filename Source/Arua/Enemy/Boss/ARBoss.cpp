// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/ARBoss.h"
#include "AI/AUAIController.h"

AARBoss::AARBoss()
{
	// 메시 컴포넌트 설정.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, 90.0f, 0.0f)
	);

	// 애셋 지정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/Elemental_Dragon/Meshes/SK_Elemental_Dragon_Toon.SK_Elemental_Dragon_Toon"));
	
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	AIControllerClass = AAUAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;



}
