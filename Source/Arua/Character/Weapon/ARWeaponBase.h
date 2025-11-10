// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARWeaponBase.generated.h"

//UENUM(BlueprintType)
//enum class EWeaponType : uint8
//{
//	None UMETA(DisplayName = "None"),
//	Sword UMETA(DisplayName = "Sword")
//};

UCLASS()
class ARUA_API AARWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AARWeaponBase();

	virtual void AttachToSocket(class ACharacter* Character, FName SocketName);
	virtual void DetachToCharacter();

	//FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; } 


public:
	// 무기 타입
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//EWeaponType WeaponType;

	// 무기 스태틱 메시
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	class UStaticMeshComponent* WeaponMesh;
	 
	// 무기를 붙일 소켓
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Weapon)
	FName AttachSocketName;

	// Idle 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimSequenceBase> IdleByWeapon;

	// 락온이 되지 않았을 때 걷기 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimSequenceBase> WalkByWeapon;

	// 달리기 모션 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimSequenceBase> RunByWeapon;

	// 어택 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage>AttackAnimByWeapon;

	// 락온 시 걷기 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UBlendSpace> LockOnWalkByWeapon;

	// 콤보 액션을 위한 액션 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UARComboActionData> ComboActionDataByWeapon;

	


	
};
