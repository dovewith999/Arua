// CommonActivatableViewBase.h
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "CommonActivatableViewBase.generated.h"

class UViewModelBase;
/**
 * CommonUI의 Activatable Widget용 View 베이스 클래스
 * 스택 기반 UI (메뉴, 팝업 등)에 사용
 * 작성자 : 임희섭
 * 작성일 : 25/11/03
 */
UCLASS(Abstract)
class ARUA_API UCommonActivatableViewBase : public UCommonActivatableWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "View")
    virtual void SetViewModel(UViewModelBase* InViewModel);

    UFUNCTION(BlueprintPure, Category = "View")
    FORCEINLINE UViewModelBase* GetViewModel() const { return ViewModel; }

protected:
	// Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
    // End UCommonActivatableWidget Interface

protected:
    UFUNCTION(BlueprintNativeEvent, Category = "View")
    void BindViewModel();
    virtual void BindViewModel_Implementation() {}

    UFUNCTION(BlueprintNativeEvent, Category = "View")
    void UnbindViewModel();
    virtual void UnbindViewModel_Implementation() {}

    UFUNCTION(BlueprintNativeEvent, Category = "View")
    void OnViewModelPropertyChanged(FName PropertyName);
    virtual void OnViewModelPropertyChanged_Implementation(FName PropertyName) {}

private:
    void HandlePropertyChanged(FName PropertyName);

protected:
    UPROPERTY(BlueprintReadOnly, Category = "View")
    TObjectPtr<UViewModelBase> ViewModel;

    FDelegateHandle PropertyChangedHandle;

private:
	bool bIsActivated = false;

};