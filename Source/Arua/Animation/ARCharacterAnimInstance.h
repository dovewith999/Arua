// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float Angle;
};
