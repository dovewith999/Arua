// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ARLoadingScreenInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType,Blueprintable, MinimalAPI)
class UARLoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARUA_API IARLoadingScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();
	//void OnLoadingScreenActivated_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
	//void OnLoadingScreenDeactivated_Implementation();
};
