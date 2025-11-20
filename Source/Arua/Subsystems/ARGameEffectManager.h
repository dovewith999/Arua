// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ARGameEffectManager.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGameEffectManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UARGameEffectManager();

public:
	UFUNCTION()
	void StartBlur();

	UFUNCTION()
	void EndBlur();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|PostProcess", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMaterialInstance> RadialBlurMaterialInstance;
	
};
