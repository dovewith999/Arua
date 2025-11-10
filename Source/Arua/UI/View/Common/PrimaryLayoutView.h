// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/CommonViewBase.h"
#include "GameplayTagContainer.h"
#include "PrimaryLayoutView.generated.h"

class UCommonActivatableWidgetContainerBase;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ARUA_API UPrimaryLayoutView : public UCommonViewBase
{
	GENERATED_BODY()

public:
	UCommonActivatableWidgetContainerBase* FindWidgetStackByTag(const FGameplayTag& InStackTag);

protected:
	UFUNCTION(BlueprintCallable)
	void RegisterWidgetStack(UPARAM(meta = (Categories = "WidgetStack")) FGameplayTag InStackTag, UCommonActivatableWidgetContainerBase* InStack);

private:
	UPROPERTY(Transient)
	TMap<FGameplayTag, UCommonActivatableWidgetContainerBase*>RegisteredWidgetStackMap;
	
};
