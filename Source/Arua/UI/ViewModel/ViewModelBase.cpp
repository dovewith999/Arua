// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/ViewModelBase.h"

void UViewModelBase::NotifyPropertyChanged(FName PropertyName)
{
	OnPropertyChanged.Broadcast(PropertyName);
}

void UViewModelBase::NotifyPropertiesChanged(const TArray<FName>& PropertyNames)
{
    for (const FName& PropertyName : PropertyNames)
    {
        OnPropertyChanged.Broadcast(PropertyName);
    }
}

void UViewModelBase::NotifyAllPropertiesChanged()
{
    // NAME_None은 모든 프로퍼티를 의미
    OnPropertyChanged.Broadcast(NAME_None);
}
