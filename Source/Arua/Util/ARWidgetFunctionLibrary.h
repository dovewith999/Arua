// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "ARWidgetFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARWidgetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Arua Widget Function Library")
	static TSoftClassPtr<class UCommonActivatableViewBase> GetAruaWidgetClassByTag(UPARAM(meta = (Categories = "Widget"))FGameplayTag InWidgetTag);
	
};
