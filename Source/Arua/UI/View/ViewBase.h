// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ViewBase.generated.h"

class UViewModelBase;
/**
 * 일반 UMG Widget용 ViewBase 클래스
 * 화면 UI와 3D 월드 위젯 모두 사용 가능
 * 작성자 : 임희섭
 * 작성일 : 25/11/03
 */
UCLASS(Abstract)
class ARUA_API UViewBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "View")
	virtual void SetViewModel(UViewModelBase* InViewModel);

	UFUNCTION(BlueprintPure, Category = "View")
	FORCEINLINE UViewModelBase* GetViewModel() const { return ViewModel; }

protected:
	// Begin UserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// end UserWidget Interface

protected:
	// ViewModel을 UI에 바인딩
	// 파생 클래스에서 구현하여 실제 바인딩 로직 작성
	UFUNCTION(BlueprintNativeEvent, Category = "View")
	void BindViewModel();
	virtual void BindViewModel_Implementation() {}

	// ViewModel 바인딩 해제
	// 파생 클래스에서 구현하여 실제 언바인딩 로직 작성
	UFUNCTION(BlueprintNativeEvent, Category = "View")
	void UnbindViewModel();
	virtual void UnbindViewModel_Implementation() {}

	//ViewModel의 Property가 변경되었을 때 호출
	//param PropertyName 변경된 Property 이름 (NAME_None이면 전체 갱신)
	UFUNCTION(BlueprintNativeEvent, Category = "View")
	void OnViewModelPropertyChanged(FName PropertyName);
	virtual void OnViewModelPropertyChanged_Implementation(FName PropertyName) {}

private:
	// ViewModel Property Changed 콜백
	void HandlePropertyChanged(FName PropertyName);

protected:
	// 현재 바인딩된 ViewModel
	UPROPERTY(BlueprintReadOnly, Category = "View")
	TObjectPtr<UViewModelBase> ViewModel;

	// ViewModel의 PropertyChanged 델리게이트 핸들
	FDelegateHandle PropertyChangedHandle;


};
