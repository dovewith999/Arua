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

	// UI 표시 여부를 결정하는 상태 추가
	bool IsSensed() const { return bIsSensed; }
	void SetIsSensed(bool bInSensed);

private:
	UPROPERTY()
	TObjectPtr<class UMonsterAttributeSet> AttributeSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FString Name;

	// UI 상태 제어 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI State", meta = (AllowPrivateAccess = "true"))
	bool bIsSensed = false;
};
