// Fill out your copyright notice in the Description page of Project Settings.

#include "CommonViewBase.h"
#include "UI/ViewModel/ViewModelBase.h"

void UCommonViewBase::SetViewModel(UViewModelBase* InViewModel)
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

    if (ViewModel)
    {
        PropertyChangedHandle = ViewModel->OnPropertyChanged.AddUObject(
            this, &UCommonViewBase::HandlePropertyChanged);

        BindViewModel();
        OnViewModelPropertyChanged(NAME_None);
    }
}

void UCommonViewBase::NativeConstruct()
{
    Super::NativeConstruct();

    if (ViewModel)
    {
        PropertyChangedHandle = ViewModel->OnPropertyChanged.AddUObject(
            this, &UCommonViewBase::HandlePropertyChanged);

        BindViewModel();
        OnViewModelPropertyChanged(NAME_None);
    }
}

void UCommonViewBase::NativeDestruct()
{
    if (ViewModel && PropertyChangedHandle.IsValid())
    {
        UnbindViewModel();
        ViewModel->OnPropertyChanged.Remove(PropertyChangedHandle);
        PropertyChangedHandle.Reset();
    }

    Super::NativeDestruct();
}

void UCommonViewBase::HandlePropertyChanged(FName PropertyName)
{
    OnViewModelPropertyChanged(PropertyName);
}