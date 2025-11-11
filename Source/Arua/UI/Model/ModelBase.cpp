// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Model/ModelBase.h"
#include "AbilitySystemComponent.h"

void UModelBase::BindAttributeChange(UAbilitySystemComponent* ASC, FGameplayAttribute Attribute, FName PropertyName)
{
    if (!ASC || !Attribute.IsValid())
    {
        return;
    }

    BoundASC = ASC;

    // 이미 바인딩되어 있다면 제거
    if (FDelegateHandle* ExistingHandle = AttributeHandles.Find(Attribute))
    {
        ASC->GetGameplayAttributeValueChangeDelegate(Attribute).Remove(*ExistingHandle);

        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("UnBind AttributeSet")));
    }

    // 새로 바인딩
    FDelegateHandle NewHandle = ASC->GetGameplayAttributeValueChangeDelegate(Attribute)
        .AddUObject(this, &UModelBase::OnAttributeChanged, PropertyName);

    AttributeHandles.Add(Attribute, NewHandle);
}

void UModelBase::UnbindAllAttributes()
{
    if (BoundASC)
    {
        for (const auto& Pair : AttributeHandles)
        {
            BoundASC->GetGameplayAttributeValueChangeDelegate(Pair.Key).Remove(Pair.Value);
        }
    }

    AttributeHandles.Empty();
    BoundASC = nullptr;
}

void UModelBase::OnAttributeChanged(const FOnAttributeChangeData& Data, FName PropertyName)
{
    // Attribute가 변경되면 해당 PropertyName으로 알림
    NotifyModelDataChanged(PropertyName);
}

void UModelBase::NotifyModelDataChanged(const FName& InPropertyName)
{
	OnModelDataChanged.Broadcast(InPropertyName);
}
