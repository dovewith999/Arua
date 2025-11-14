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

	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, ChargeCount)
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxChargeCount)
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, Stemina)
	ATTRIBUTE_ACCESSORS_BASIC(UPlayerAttributeSet, MaxStemina)


public:
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ChargeCount", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ChargeCount;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|ChargeCount", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxChargeCount;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stemina", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Stemina;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Stemina", meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxStemina;
};
