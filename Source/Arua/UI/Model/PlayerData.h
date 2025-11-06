// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Model/ModelBase.h"
#include "PlayerData.generated.h"

/**
 * Player의 Data를 가지는 Model
 * 작성자 : 임희섭
 * 작성일 : 25/11/04
 */
UCLASS()
class ARUA_API UPlayerData : public UModelBase
{
	GENERATED_BODY()

public:
	// Todo : 캐릭터가 만들어지면 사라질 코드, 임시 값 세팅
	void Initialize();

	UFUNCTION()
	void BindToAttributeSet(class UPlayerAttributeSet* InAttributeSet);

public:
	float GetHp() const;
	float GetMaxHp() const;
	float GetHpRatio() const;

	void SetHp(float InNewHp);
	void SetMaxHp(float InNewMaxHp);
	void TakeDamage(float InDamage);

private:
	UPROPERTY()
	TObjectPtr<class UPlayerAttributeSet> AttributeSet;
};
