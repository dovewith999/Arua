// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Model/ModelBase.h"
#include "InteractObjectData.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UInteractObjectData : public UModelBase
{
	GENERATED_BODY()

public:
	FORCEINLINE FString GetName() const { return Name; }
	void SetName(const FString& InName);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	FString Name;
	
};
