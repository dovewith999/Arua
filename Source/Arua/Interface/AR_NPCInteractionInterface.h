// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AR_NPCInteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAR_NPCInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class ARUA_API IAR_NPCInteractionInterface
{
	GENERATED_BODY()

public:
	virtual void PlayInteraction(APawn* InInteractor) = 0;
};
