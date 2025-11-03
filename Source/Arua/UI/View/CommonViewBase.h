// CommonViewBase.h
#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonViewBase.generated.h"

class UViewModelBase;
/**
 * CommonUI용 View 베이스 클래스
 * CommonUI의 기능을 활용하는 화면 UI 전용
 * 작성자 : 임희섭
 * 작성일 : 25/11/03
 */
UCLASS(Abstract)
class ARUA_API UCommonViewBase : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "View")
	virtual void SetViewModel(UViewModelBase* InViewModel);

	UFUNCTION(BlueprintPure, Category = "View")
	FORCEINLINE UViewModelBase* GetViewModel() const { return ViewModel; }

protected:
	// Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget Interface

protected:
	UPROPERTY(BlueprintReadOnly, Category = "View")
	TObjectPtr<UViewModelBase> ViewModel;

	FDelegateHandle PropertyChangedHandle;
	// ViewModel 바인딩
	UFUNCTION(BlueprintNativeEvent, Category = "View")
	void BindViewModel();
	virtual void BindViewModel_Implementation() {}

	 // ViewModel 언바인딩
	UFUNCTION(BlueprintNativeEvent, Category = "View")
	void UnbindViewModel();
	virtual void UnbindViewModel_Implementation() {}

	//Property 변경 처리
	UFUNCTION(BlueprintNativeEvent, Category = "View")
	void OnViewModelPropertyChanged(FName PropertyName);
	virtual void OnViewModelPropertyChanged_Implementation(FName PropertyName) {}

private:
	void HandlePropertyChanged(FName PropertyName);
};