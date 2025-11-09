// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "ARCommonButtonBase.generated.h"

/**
 * 프로젝트에서 CommonButtonBase로 사용할 클래스 추가 정의
 * 작성자 : 임희섭
 * 작성일 : 25/11/09
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class ARUA_API UARCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

private:
	virtual void NativePreConstruct() override;

	virtual void NativeOnCurrentTextStyleChanged() override;

private:
	/*BoundWidget*/
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<class UCommonTextBlock> CommonTextBlock_ButtonText;

	UPROPERTY(EditAnywhere ,BlueprintReadOnly, Category = "Frontend Buttton", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Buttton", meta = (AllowPrivateAccess = "true"))
	bool bUserUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Buttton", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};
