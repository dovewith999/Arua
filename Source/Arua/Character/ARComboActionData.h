// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ARComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UARComboActionData();

	UPROPERTY(EditAnywhere,Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category= Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category =Name)
	TArray<float> FrameRate;

	UPROPERTY(EditAnywhere,Category =Name)
	TArray<float> EffectiveFrameCount;
	
};
