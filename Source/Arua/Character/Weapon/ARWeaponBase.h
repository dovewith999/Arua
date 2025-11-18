// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTables/ARWeaponData.h"
#include "NativeGameplayTags.h"
#include "ARWeaponBase.generated.h"


UCLASS()
class ARUA_API AARWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARWeaponBase();

	// 무기를 장착하는 함수
	virtual void AttachToSocket(class ACharacter* Character, FName SocketName);
	// 무기를 해제하는 함수
	virtual void DetachFromCharacter();

	virtual void InitializeFromData();

	//FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; } 

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class UDataTable> WeaponDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FName WeaponName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	FARWeaponData WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TObjectPtr<class UStaticMeshComponent> WeaponStaticMesh;

	// 무기 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FGameplayTag WeaponTag;

	// 무기 장착 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> WeaponEquipMontage;

	// 무기에 따른 AnimBp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimInstance> AnimInstanceByWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Socket)
	FName Socket;
	
};
