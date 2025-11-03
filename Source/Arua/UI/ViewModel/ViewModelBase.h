// ViewModelBase.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ViewModelBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPropertyChanged, FName);

/**
 * WPF의 INotifyPropertyChanged 패턴을 구현한 ViewModel 베이스 클래스
 * 모든 ViewModel은 이 클래스를 상속받아 구현
 * 작성자 : 임희섭
 * 작성일 : 25/11/03
 */
UCLASS(Blueprintable, BlueprintType)
class ARUA_API UViewModelBase : public UObject
{
    GENERATED_BODY()

public:
    // Property Changed 델리게이트 (PropertyName을 전달)
    FOnPropertyChanged OnPropertyChanged;

protected:
    //Property가 변경되었음을 알림

    UFUNCTION(BlueprintCallable, Category = "ViewModel")
    void NotifyPropertyChanged(FName PropertyName);

    // 여러 Property가 변경되었음을 알림
    UFUNCTION(BlueprintCallable, Category = "ViewModel")
    void NotifyPropertiesChanged(const TArray<FName>& PropertyNames);

     //모든 Property가 변경되었음을 알림 (전체 갱신)
    UFUNCTION(BlueprintCallable, Category = "ViewModel")
    void NotifyAllPropertiesChanged();
};