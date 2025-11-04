// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/View/CommonViewBase.h"
#include "PlayerHUDView.generated.h"

/**
 * 플레이어의 HUD 클래스
 * 작성자 : 임희섭
 * 작성일 : 25/11/04
 */
UCLASS()
class ARUA_API UPlayerHUDView : public UCommonViewBase
{
	GENERATED_BODY()

public:
	UPlayerHUDView();

public:
	void SetViewModelChildWidget(class UViewModelBase* InViewModel);

protected:
	// Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget Interface

protected:
	// ViewModel 바인딩
	virtual void BindViewModel_Implementation() override;

	// ViewModel 언바인딩
	virtual void UnbindViewModel_Implementation() override;

	//Property 변경 처리
	virtual void OnViewModelPropertyChanged_Implementation(FName PropertyName) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD|HpBar", meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UHpBarView> HpBar;
};
