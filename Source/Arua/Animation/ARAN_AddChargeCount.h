// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "ARAN_AddChargeCount.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARAN_AddChargeCount : public UAnimNotify
{
	GENERATED_BODY()

public:
	UARAN_AddChargeCount();

protected:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	UPROPERTY(EditAnywhere)
	struct FGameplayTag TriggerGameplayTag;
};
