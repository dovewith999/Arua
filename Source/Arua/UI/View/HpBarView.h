// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/ViewBase.h"
#include "HpBarView.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API UHpBarView : public UViewBase
{
	GENERATED_BODY()

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
	TObjectPtr<class UProgressBar> HpProgressBar;
	
};
