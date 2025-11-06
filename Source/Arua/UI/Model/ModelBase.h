// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttributeSet.h"
#include "ModelBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnModelDataChanged, const FName&);


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
	void BindAttributeChange(class UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, FName PropertyName);
	
	//모든 Attribute 바인딩 해제
	UFUNCTION(BlueprintCallable, Category = "Model")
	void UnbindAllAttributes();

protected:
	UFUNCTION(BlueprintCallable, Category = "Model")
	void NotifyModelDataChanged(const FName& InPropertyName);

private:
	void OnAttributeChanged(const struct FOnAttributeChangeData& Data, FName PropertyName);

	// Attribute 델리게이트 핸들 저장
	TMap<FGameplayAttribute, FDelegateHandle> AttributeHandles;

	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> BoundASC;
};
