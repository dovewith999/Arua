// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/ARWeaponBase.h"
#include "GameFramework/Character.h"

// Sets default values
AARWeaponBase::AARWeaponBase()
{
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponStaticMesh;
}

void AARWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDataTable && WeaponName != NAME_None)
	{
		InitializeFromData();
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

void AARWeaponBase::InitializeFromData()
{
	const FARWeaponData* Row = WeaponDataTable->FindRow<FARWeaponData>(WeaponName, TEXT("Weapon Initialize"));

	WeaponData = *Row;

	if (UStaticMesh* WeaponMesh = Row->WeaponMeshAsset.LoadSynchronous())
	{
		WeaponStaticMesh->SetStaticMesh(WeaponMesh);
	}
}


