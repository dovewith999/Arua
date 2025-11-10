// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/ARWeaponBase.h"
#include "GameFramework/Character.h"

// Sets default values
AARWeaponBase::AARWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	AttachSocketName = "hand_rSocket";

	static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleByWeaponRef(TEXT(""));
	if (IdleByWeaponRef.Object)
	{
		IdleByWeapon = IdleByWeaponRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> WalkByWeaponRef(TEXT(""));
	if (IdleByWeaponRef.Object)
	{
		IdleByWeapon = IdleByWeaponRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimSequence> RunByWeaponRef(TEXT(""));
	if (RunByWeaponRef.Object)
	{
		IdleByWeapon = RunByWeaponRef.Object;
	}

}

void AARWeaponBase::AttachToSocket(class ACharacter* Character, FName SocketName)
{
	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
	if (Character)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}


}

void AARWeaponBase::DetachToCharacter()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}


