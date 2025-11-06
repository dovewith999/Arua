// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Model/ModelBase.h"
#include "BossData.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UBossData : public UModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void BindToAttributeSet(class UMonsterAttributeSet* InAttributeSet);

public:
	float GetHp() const;
	float GetMaxHp() const;
	float GetHpRatio() const;
	FString GetName() const;

	void SetHp(float InNewHp);
	void SetMaxHp(float InNewMaxHp);
	void SetName(const FString& InName);
	void TakeDamage(float InDamage);

private:
	UPROPERTY()
	TObjectPtr<class UMonsterAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FString Name;
};
