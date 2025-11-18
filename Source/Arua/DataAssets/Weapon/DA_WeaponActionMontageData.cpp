// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Weapon/DA_WeaponActionMontageData.h"

UAnimMontage* UDA_WeaponActionMontageData::FindWeaponMontage(FGameplayTag WeaponTag, FGameplayTag ActionTag) const
{
    for (const auto& Item : WeaponActionMontages)
    {
        if (Item.WeaponTag == WeaponTag && Item.ActionTag == ActionTag)
        {
            return Item.Montage;
        }
    }
    return nullptr;
}

UAnimMontage* UDA_WeaponActionMontageData::FindCommonMontage(FGameplayTag ActionTag) const
{
    const auto* Found = CommonMontages.Find(ActionTag);
    return Found ? *Found : nullptr;
}
