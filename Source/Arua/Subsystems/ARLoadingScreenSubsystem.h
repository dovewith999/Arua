// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ARLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARLoadingScreenSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Begin USubSystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	// End USubSystem Interface

private:
	// 맵이 로드되기 이전에 실행될 함수
	UFUNCTION()
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName); 
	
	// 맵이 로드된 이후에 실행된 함수
	UFUNCTION()
	void OnMapPostLoaded(UWorld* LoadedWorld);
};
