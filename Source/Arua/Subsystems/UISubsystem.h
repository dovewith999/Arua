// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

struct FGameplayTag;

enum class EAsyncPushWidgetState : uint8
{
	OnCreatedBeforePush,
	AfterPush
};
/**
 * 
 */
UCLASS()
class ARUA_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UUISubsystem* Get(const UObject* WorldContextObject);

	// Begin USubsyem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	// End USubsyem Interface

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(class UPrimaryLayoutView* InCreatedWidget);

	void PushSoftWidgetToSackAsync(
	const FGameplayTag& InWidgetStackTag, 
	TSoftClassPtr<class UCommonActivatableViewBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, TObjectPtr<class UCommonActivatableViewBase>)> AsyncPushStateCallback
	);

private:
	UPROPERTY(Transient)
	TObjectPtr<class UPrimaryLayoutView> CreatedPrimaryLayout;
	
};
