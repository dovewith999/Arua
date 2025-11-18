// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/ARWeaponBase.h"
#include "GameFramework/Character.h"

// Sets default values
AARWeaponBase::AARWeaponBase()
{
	WeaponStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	RootComponent = WeaponStaticMesh;
	WeaponStaticMesh->SetSimulatePhysics(false);
	this->SetActorEnableCollision(false);
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
	if (Character->GetMesh())
	{
		//if (!Character)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Character is NULL"));
		//	return;
		//}

		//UE_LOG(LogTemp, Log, TEXT("Character OK"));

		//// Mesh
		//if (!Character->GetMesh())
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Character->GetMesh() is NULL!"));
		//	return;
		//}

		//// Socket
		//if (!Character->GetMesh()->DoesSocketExist(SocketName))
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Socket %s does NOT exist!"), *SocketName.ToString());
		//	return;
		//}

		//// Root
		//if (!RootComponent)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Weapon RootComponent is NULL!"));
		//	return;
		//}

		//UE_LOG(LogTemp, Log, TEXT("Attach OK!"));

		//AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		GetWorld()->GetTimerManager().SetTimerForNextTick([this, Character, SocketName]()
			{
				AttachToComponent(Character->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					SocketName);
			});


	}
}

void AARWeaponBase::DetachFromCharacter()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AARWeaponBase::InitializeFromData()
{
	const FARWeaponData* Row = WeaponDataTable->FindRow<FARWeaponData>(WeaponName, TEXT("Weapon Initialize"));

	WeaponData = *Row;

	// 스태틱메시 초기화
	if (UStaticMesh* WeaponMesh = Row->WeaponMeshAsset.LoadSynchronous())
	{
		WeaponStaticMesh->SetStaticMesh(WeaponMesh);
	}

	FName TagName = Row->WeaponTag;
	WeaponTag = FGameplayTag::RequestGameplayTag(TagName);

	FName SocketName = Row->Socket;
	Socket = SocketName;

	

	/*if (UAnimMontage* WeaponEquipMontageTemp = Row->WeaponEquipMontageData.LoadSynchronous())
	{
		WeaponEquipMontage = WeaponEquipMontageTemp;
	}

	if (UAnimInstance* AnimInstanceByWeaponTempRef = Row->AnimInstanceByWeaponData.LoadSynchronous())
	{
		AnimInstanceByWeapon = AnimInstanceByWeaponTempRef;
	}*/


}


