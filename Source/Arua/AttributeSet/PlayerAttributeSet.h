// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ARAttributeSetBase.h"

#include "PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UPlayerAttributeSet : public UARAttributeSetBase
{
	GENERATED_BODY()

public:
	UPlayerAttributeSet();


public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

};
