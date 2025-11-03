// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonActivatableViewBase.h"
#include "UI/ViewModel/ViewModelBase.h"

void UCommonActivatableViewBase::SetViewModel(UViewModelBase* InViewModel)
{
    if (ViewModel)
    {
        UnbindViewModel();
        if (PropertyChangedHandle.IsValid())
        {
            ViewModel->OnPropertyChanged.Remove(PropertyChangedHandle);
            PropertyChangedHandle.Reset();
        }
    }

    ViewModel = InViewModel;

    if (ViewModel && bIsActivated)
    {
        PropertyChangedHandle = ViewModel->OnPropertyChanged.AddUObject(
            this, &UCommonActivatableViewBase::HandlePropertyChanged);

        BindViewModel();
        OnViewModelPropertyChanged(NAME_None);
    }
}

void UCommonActivatableViewBase::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCommonActivatableViewBase::NativeDestruct()
{
    if (ViewModel && PropertyChangedHandle.IsValid())
    {
        UnbindViewModel();
        ViewModel->OnPropertyChanged.Remove(PropertyChangedHandle);
        PropertyChangedHandle.Reset();
    }

    Super::NativeDestruct();
}

void UCommonActivatableViewBase::NativeOnActivated()
{
    Super::NativeOnActivated();

    bIsActivated = true;

    // Activate 시 ViewModel 바인딩
    if (ViewModel)
    {
        PropertyChangedHandle = ViewModel->OnPropertyChanged.AddUObject(
            this, &UCommonActivatableViewBase::HandlePropertyChanged);

        BindViewModel();
        OnViewModelPropertyChanged(NAME_None);
    }
}

void UCommonActivatableViewBase::NativeOnDeactivated()
{
    // Deactivate 시 ViewModel 언바인딩
    if (ViewModel && PropertyChangedHandle.IsValid())
    {
        UnbindViewModel();
        ViewModel->OnPropertyChanged.Remove(PropertyChangedHandle);
        PropertyChangedHandle.Reset();
    }

    bIsActivated = false;

    Super::NativeOnDeactivated();
}

void UCommonActivatableViewBase::HandlePropertyChanged(FName PropertyName)
{
    OnViewModelPropertyChanged(PropertyName);
}