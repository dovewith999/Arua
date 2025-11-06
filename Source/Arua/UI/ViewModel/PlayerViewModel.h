// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "UI/Model/PlayerData.h" // Model 참조
#include "PlayerViewModel.generated.h"

/**
 * 테스트용 ViewModel
 * 작성자 : 임희섭
 * 작성일 : 25/11/04
 */
UCLASS(Blueprintable, BlueprintType)
class ARUA_API UPlayerViewModel : public UViewModelBase
{
	GENERATED_BODY()
	
public:
	void Initialize(UModelBase* InModel)
	{
		Model = Cast<UPlayerData>(InModel);

		if (Model)
		{
			Model->OnModelDataChanged.AddUObject(this, &UPlayerViewModel::OnModelDataChanged);
		}
	}

public:
	UFUNCTION(BlueprintCallable)
	void SetHp(float NewHp)
	{
		Model->SetHp(NewHp);
		NotifyPropertyChanged(FName(TEXT("Hp")));
	}

	UFUNCTION(BlueprintCallable)
	void SetMaxHp(float NewMaxHp)
	{
		Model->SetMaxHp(NewMaxHp);
		NotifyPropertyChanged(FName(TEXT("MaxHp")));
	}

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float InDamage)
	{
		Model->TakeDamage(InDamage);
		NotifyPropertyChanged(FName(TEXT("Hp")));
	}

public:
	UFUNCTION(BlueprintCallable)
	float GetHpRatio() const
	{
		return Model->GetHpRatio();
	}

private:
	UFUNCTION()
	void OnModelDataChanged(const FName& PropertyName)
	{
		NotifyPropertyChanged(PropertyName);
	}

private:
	UPROPERTY()
	TObjectPtr<UPlayerData> Model;
};
