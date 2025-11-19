// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ARSectionEndAnimNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSectionEnd, FName, SectionName);

/**
 * 
 */
UCLASS()
class ARUA_API UARSectionEndAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnSectionEnd OnSectionEnd;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SectionName;

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
