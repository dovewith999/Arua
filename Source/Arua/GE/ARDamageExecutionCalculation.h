// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectCalculation.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ARDamageExecutionCalculation.generated.h"

/**
 * 데미지 처리에 이용할 클래스
 * 작성자 : 임희섭
 * 작성일 : 25/11/06
 */
UCLASS()
class ARUA_API UARDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
    UARDamageExecutionCalculation();

    virtual void Execute_Implementation(
        const FGameplayEffectCustomExecutionParameters& ExecutionParams,
        OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput
    ) const override;
	
};
