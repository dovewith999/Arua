// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AruaPlayerController.generated.h"

class UPlayerViewModel;
class UPlayerHUDView;
/**
 * 
 */
UCLASS()
class ARUA_API AAruaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAruaPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UPlayerViewModel> ViewModel;

	UPROPERTY()
	TObjectPtr<UPlayerHUDView> HUD;

	UPROPERTY()
	TSubclassOf<UPlayerHUDView> HUDClass;
};
