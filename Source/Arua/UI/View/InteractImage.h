// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/ViewBase.h"
#include "InteractImage.generated.h"

/**
 * 상호 작용 가능한 오브젝트를 탐지하면 화면 HUD에 보여주는 View
 * 작성자 : 임희섭
 * 작성일 : 25/11/13
 */
UCLASS()
class ARUA_API UInteractImage : public UViewBase
{
	GENERATED_BODY()

public:
	UInteractImage();

protected:
	// Begin UserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// end UserWidget Interface

protected:
	// ViewModel을 UI에 바인딩
	virtual void BindViewModel_Implementation() override;

	// ViewModel 바인딩 해제
	virtual void UnbindViewModel_Implementation() override;

	//ViewModel의 Property가 변경되었을 때 호출
	virtual void OnViewModelPropertyChanged_Implementation(FName PropertyName) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Message;


	
};
