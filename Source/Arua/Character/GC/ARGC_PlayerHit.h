// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ARGC_PlayerHit.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARGC_PlayerHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UARGC_PlayerHit();

	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameplayCue)
	TObjectPtr<class UAnimMontage> HitActionMontage;
	
	
};
