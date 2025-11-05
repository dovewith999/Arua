// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterBase.h"
#include "InputActionValue.h"
#include "ARCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARCharacterPlayer : public AARCharacterBase
{
	GENERATED_BODY()

public:
	AARCharacterPlayer();

	virtual void BeginPlay() override;

	virtual void SetDead() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool GetRunState() { return bIsRunning; }

	FORCEINLINE bool GetWalkState() { return bIsWalking; }

	virtual void PossessedBy(AController* NewController) override;

	// 카메라 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camear, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	// 입력 관련 섹션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRunning : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsWalking : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRolling : 1;

	void Move(const FInputActionValue& Value);
	void NotMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void RunTriggered(const FInputActionValue& Value);
	void RunComplete(const FInputActionValue& Value);
	void Roll(const FInputActionValue& Value);
	void RollCompleted();

	// 애니메이션 섹션
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Animation)
	TObjectPtr<class UAnimMontage> RollActionMontage;

	FTimerHandle RollAnimTimer;

	// 무기 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;

	// ASC 어빌리티 섹션
protected:
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;
};
