// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/View/ViewBase.h"
#include "UI/ViewModel/ViewModelBase.h"
#include "CommonViewBase.h"

void UViewBase::SetViewModel(UViewModelBase* InViewModel)
{
    // 기존 ViewModel 언바인딩
    if (ViewModel)
    {
        UnbindViewModel();
        if (PropertyChangedHandle.IsValid())
        {
            ViewModel->OnPropertyChanged.Remove(PropertyChangedHandle);
            PropertyChangedHandle.Reset();
        }
    }

    // 새 ViewModel 설정
    ViewModel = InViewModel;

    // 새 ViewModel 바인딩
    if (ViewModel)
    {
        PropertyChangedHandle = ViewModel->OnPropertyChanged.AddUObject(
            this, &UViewBase::HandlePropertyChanged);

        BindViewModel();

        // 초기 데이터 로드
        OnViewModelPropertyChanged(NAME_None);
    }
}
void UViewBase::NativeConstruct()
{
    Super::NativeConstruct();

    // ViewModel이 이미 설정되어 있다면 바인딩
    if (ViewModel)
    {
        PropertyChangedHandle = ViewModel->OnPropertyChanged.AddUObject(
            this, &UViewBase::HandlePropertyChanged);

        BindViewModel();
        OnViewModelPropertyChanged(NAME_None);
    }
}

void UViewBase::NativeDestruct()
{
    // ViewModel 언바인딩
    if (ViewModel && PropertyChangedHandle.IsValid())
    {
        UnbindViewModel();
        ViewModel->OnPropertyChanged.Remove(PropertyChangedHandle);
        PropertyChangedHandle.Reset();
    }

    Super::NativeDestruct();
}

void UViewBase::HandlePropertyChanged(FName PropertyName)
{
    OnViewModelPropertyChanged(PropertyName);
}