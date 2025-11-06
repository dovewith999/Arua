// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "ARAttributeSetBase.generated.h"

/**
 *
 */
UCLASS()
class ARUA_API UARAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UARAttributeSetBase();

public:
	ATTRIBUTE_ACCESSORS_BASIC(UARAttributeSetBase, Health)
	ATTRIBUTE_ACCESSORS_BASIC(UARAttributeSetBase, MaxHealth)

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;

};
