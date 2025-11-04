// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

// 키 입력 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequestInput);

UCLASS()
class ARUA_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 키 다운 콜백 함수
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	// 마우스 버튼 콜백 함수
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	// 키 입력 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FOnRequestInput OnRequestInput;
};
