// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/ViewBase.h"
#include "BossHpBarView.generated.h"

/**
 * HUD에 보스의 이름, Hp를 띄울 View 클래스이다.
 * 작성자 : 임희섭
 * 작성일 : 25/11/06
 */
UCLASS()
class ARUA_API UBossHpBarView : public UViewBase
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameText;
	
};
