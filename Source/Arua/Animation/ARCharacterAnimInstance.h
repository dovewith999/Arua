// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataTables/ARWeaponData.h"
#include "GameplayTagContainer.h"
#include "DataAssets/Weapon/DA_WeaponActionMontageData.h"
#include "ARCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UARCharacterAnimInstance();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages")
	UDA_WeaponActionMontageData* MontageDataAsset;

	// 몽타주 재생
	UFUNCTION(BlueprintCallable)
	void PlayActionMontage(const FGameplayTag& WeaponTag, const FGameplayTag& ActionTag);

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimes) override;


	// 애님 인스턴스를 소유하는 객체의 정보를 담는 변수.
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float WalkSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Character)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRun : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsWalk : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	uint8 bIsWeaponChanged : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FGameplayTag CurrentWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Character)
	int32 StateMachineIndex;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Character)
	uint8 bASCInitialized : 1;

	TMap<FGameplayTag, TMap<FGameplayTag, UAnimMontage*>> WeaponActionMontageTable;
	TMap<FGameplayTag, UAnimMontage*> CommonMontageTable;


protected:
	UFUNCTION()
	void OnGameplayTagChanged(const FGameplayTag Tag, int32 NewCount);

	UFUNCTION()
	int32 GetWeaponLayerIndex() const;


};
