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

	// 카메라 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camear, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;


};
