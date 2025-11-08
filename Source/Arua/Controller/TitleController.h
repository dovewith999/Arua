// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleController.generated.h"

/**
 *
 */
UCLASS()
class ARUA_API ATitleController : public APlayerController
{
	GENERATED_BODY()

public:
	// Begin APlayerController Interface
	virtual void OnPossess(APawn* aPawn) override;
	// End APlayerController Interface
};
