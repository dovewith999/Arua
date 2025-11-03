// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModelBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnModelDataChanged);


/**
 * 게임 로직과 데이터를 담당하는 순수한 데이터 모델
 * ViewModel과는 독립적으로 동작
 * 작성자 : 임희섭
 * 작성일 : 25/11/03
 */
UCLASS(Blueprintable, BlueprintType)
class ARUA_API UModelBase : public UObject
{
	GENERATED_BODY()

public:
	FOnModelDataChanged OnModelDataChanged;

protected:
	UFUNCTION(BlueprintCallable, Category = "Model")
	void NotifyModelDataChanged();
};
