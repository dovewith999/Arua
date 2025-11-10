// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ARLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig)
class ARUA_API UARLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassCheck() const;

	UPROPERTY(Config, EditAnywhere ,Category = "Loading Screen Settings")
	TSoftClassPtr<class UUserWidget> SoftLoadingScreenWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	float HoldLoadingScreenExtraSceonds = 3.f;

	UPROPERTY(Config, EditAnywhere, Category = "Loading Screen Settings")
	bool bShouldLoadingScreenInEditor = false;
	
};
