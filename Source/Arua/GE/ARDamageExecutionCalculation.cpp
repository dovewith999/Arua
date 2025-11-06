// Fill out your copyright notice in the Description page of Project Settings.


#include "ARDamageExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/ARAttributeSetBase.h"

struct FDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Attack)
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health)

    FDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UARAttributeSetBase, Attack, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UARAttributeSetBase, Health, Target, false);
    }
};

static const FDamageStatics& DamageStatics()
{
    static FDamageStatics DStatics;
    return DStatics;
}

UARDamageExecutionCalculation::UARDamageExecutionCalculation()
{
    // 필요한 Attribute들을 등록
    RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
    RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
}

void UARDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent(); // 타겟의 ASC
    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent(); // Owener의 ASC

    AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    // Attribute 값 가져오기
    float Attack = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
        DamageStatics().AttackDef, EvaluationParameters, Attack);

    // 데미지 계산 
    // Todo : 추후에 공식 계산해서 넣을 예정
    float FinalDamage = Attack;

    // Health Attribute에 데미지 적용
    OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
        DamageStatics().HealthProperty, EGameplayModOp::Additive, -FinalDamage));
}
