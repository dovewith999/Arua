// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "Character/GA/TA/ARTA_TraceBase.h"
#include "AnimNotify_GASAttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UAnimNotify_GASAttackHitCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_GASAttackHitCheck();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag TriggerGameplayTag;

	// 전달할 타겟팅 액터 클래스 변수 추가 - 25/11/13 임희섭
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	TSubclassOf<class AARTA_TraceBase> TargetActorClass;

};
