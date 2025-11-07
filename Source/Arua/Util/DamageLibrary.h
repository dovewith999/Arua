// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DamageLibrary.generated.h"

/**
 * 범용적으로 대미지를 주는 함수를 호출할 수 있도록 하는 클래스
 * 작성자 : 임희섭
 * 작성일 : 25/11/07
 */
UCLASS()
class ARUA_API UDamageLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Combat|Damage")
    static bool ApplyDamage(
        class UAbilitySystemComponent* SourceASC, // 대미지를 주는 액터의 ASC
        AActor* DamageTarget,               // 대미지를 받는 타겟 액터
        float BaseDamage = 0.f,             // Attack에 더할 대미지
        float DamageMultiplier = 1.0f       // 최종값에 곱할 값
    );

protected:
    // 내부 헬퍼 함수들
    static class UAbilitySystemComponent* GetASC(AActor* Actor);
    static TSubclassOf<class UGameplayEffect> GetDamageEffectClass();
};
