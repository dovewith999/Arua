// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "AsyncAction_PushSoftWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPushSoftWidgetDelegate, class UCommonActivatableViewBase*, PushedWidget);

/**
 *
 */
UCLASS()
class ARUA_API UAsyncAction_PushSoftWidget : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObjet", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Push Soft Widget To Widget Stack"))
	static UAsyncAction_PushSoftWidget* PushSoftWidget(
	const UObject* WorldContextObject, 
	APlayerController* OwningPlayerController,
	TSoftClassPtr<class UCommonActivatableViewBase> InSoftWidgetClass, 
	UPARAM(meta = (Categories = "WidgetStack")) FGameplayTag InWidgetStackTag,
	bool bFocusOnNewlyPushedWidget = true);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate OnWidgetCreatedBeforePush;

	UPROPERTY(BlueprintAssignable)
	FOnPushSoftWidgetDelegate AfterPush;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	TWeakObjectPtr<APlayerController> CachedOwningPC;
	TSoftClassPtr<class UCommonActivatableViewBase> CachedSoftWidgetClass;
	FGameplayTag CachedWidgetStackTag;
	bool bCachedFocusOnNewlyPushedWidget = false;
};
