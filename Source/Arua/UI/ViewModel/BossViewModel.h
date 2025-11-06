// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "BossViewModel.generated.h"

/**
 * 보스 몬스터의 데이터를 관리할 VM
 * 작성자 : 임희섭
 * 작성일 : 25/11/06
 */
UCLASS()
class ARUA_API UBossViewModel : public UViewModelBase
{
	GENERATED_BODY()

public:
	void Initialize(class UModelBase* InModel);

public:
	UFUNCTION(BlueprintCallable)
	void SetHp(float NewHp);

	UFUNCTION(BlueprintCallable)
	void SetMaxHp(float NewMaxHp);

	UFUNCTION(BlueprintCallable)
	void SetName(const FString& InNewName);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float InDamage);

public:
	UFUNCTION(BlueprintCallable)
	float GetHpRatio() const;

	UFUNCTION()
	FString GetName() const;

private:
	UFUNCTION()
	void OnModelDataChanged(const FName& PropertyName);

private:
	UPROPERTY()
	TObjectPtr<class UBossData> Model;
	
};
