// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ARGameEffectManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ARUA_API UARGameEffectManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UARGameEffectManager();

public:
	FORCEINLINE TSubclassOf<class UCameraShakeBase> GetImpactCameraShakeClass() const { return ImpactCameraShakeClass; }	

public:
#pragma region BlurEffect
	UFUNCTION()
	void StartBlur();

	UFUNCTION()
	void EndBlur();
#pragma endregion

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|PostProcess", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInstance> RadialBlurMaterialInstance;

	UPROPERTY(EditAnywhere, Category = "Effect|CameraShake", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> ImpactCameraShakeClass;
};
