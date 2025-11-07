// Fill out your copyright notice in the Description page of Project Settings.


#include "Util/DamageLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

bool UDamageLibrary::ApplyDamage(UAbilitySystemComponent* SourceASC, AActor* DamageTarget, float BaseDamage, float DamageMultiplier)
{
	if (!DamageTarget || !SourceASC)
	{
		return false;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(DamageTarget);

	if (!TargetASC)
	{
		return false;
	}
	
	FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(SourceASC->GetAvatarActor());

	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		UDamageLibrary::GetDamageEffectClass(), 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		// SetByCaller로 데미지 값 전달
		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Data.Damage.Base")), BaseDamage);

		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Data.Damage.Multiplier")), DamageMultiplier);

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Apply GameplayEffect")); // 화면출력

		SourceASC->ApplyGameplayEffectSpecToTarget(
			*SpecHandle.Data.Get(), TargetASC);

		return true;
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("SpecHandle Is Not valid")); // 화면출력

	}

	return false;
}

UAbilitySystemComponent* UDamageLibrary::GetASC(AActor* Actor)
{
	return UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
}

TSubclassOf<UGameplayEffect> UDamageLibrary::GetDamageEffectClass()
{
	// FSoftClassPath로 직접 로드
	static TSubclassOf<UGameplayEffect> CachedClass = nullptr;

	if (!CachedClass)
	{
		FSoftClassPath ClassPath(TEXT("/Game/Blueprints/GE/BPGE_Damage.BPGE_Damage_C"));
		CachedClass = ClassPath.TryLoadClass<UGameplayEffect>();

		if (!CachedClass)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load Damage Effect Class"));
		}
	}

	return CachedClass;
}
