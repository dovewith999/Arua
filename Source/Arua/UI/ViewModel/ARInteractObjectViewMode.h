// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "ARInteractObjectViewMode.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UARInteractObjectViewModel : public UViewModelBase
{
	GENERATED_BODY()
	
public:
	void Initialize(class UModelBase* InModel);
	FString GetName() const;


private:
	UFUNCTION()
	void OnModelDataChanged(const FName& PropertyName);

private:
	UPROPERTY()
	TObjectPtr<class UInteractObjectData > Model;
};
