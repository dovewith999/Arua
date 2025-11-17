// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimMontage.h"
#include "DA_WeaponActionMontageData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponActionMontage
{
    GENERATED_BODY()

    // 무기 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag WeaponTag;

    // 액션 태그 (Attack, Hit, Roll 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag ActionTag;

    // 실제 재생할 몽타주
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UAnimMontage* Montage;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ARUA_API UDA_WeaponActionMontageData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    // 몽타주 리스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FWeaponActionMontage> WeaponActionMontages;

    // 공용 몽타주 (Dead 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, UAnimMontage*> CommonMontages;
	
};
