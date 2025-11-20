// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameEffectManager.h"
#include "Materials/MaterialInstance.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/BlendableInterface.h"
#include "EngineUtils.h"
#include "Engine/RendererSettings.h"
#include "GameFramework/GameUserSettings.h"

UARGameEffectManager::UARGameEffectManager()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> RadialBlurMatFinder(TEXT("/Game/Personal/LIM_H_S/Material/M_RadialBlur_Inst.M_RadialBlur_Inst"));

	if (RadialBlurMatFinder.Succeeded())
	{
		RadialBlurMaterialInstance = RadialBlurMatFinder.Object;
	}
}

void UARGameEffectManager::StartBlur()
{
	if (RadialBlurMaterialInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Start Blur"));

		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
				static IConsoleVariable* CVarAntiAliasing = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
				if (CVarAntiAliasing)
				{
					CVarAntiAliasing->Set(0, ECVF_SetByCode);
					UE_LOG(LogTemp, Warning, TEXT("AntiAliasing set to None"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to find r.AntiAliasingMethod"));
				}

				//blending list setting!
				TArray<FWeightedBlendable>& blendables = postProcessVolume->Settings.WeightedBlendables.Array;

				// if insert no add
				bool alreadyAdded = false;
				for (const FWeightedBlendable& blendable : blendables)
				{
					if (blendable.Object == RadialBlurMaterialInstance)
					{
						alreadyAdded = true;
						break;
					}
				}
				if (!alreadyAdded)
				{
					if (blendables.Num() >= 1)
					{
						blendables.Insert(FWeightedBlendable(1.0f, RadialBlurMaterialInstance), 1);
					}
					else
					{
						blendables.Add(FWeightedBlendable(1.0f, RadialBlurMaterialInstance));
					}
				}
			}
		}
	}
}

void UARGameEffectManager::EndBlur()
{
	if (RadialBlurMaterialInstance)
	{
		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
				TArray<FWeightedBlendable>& blendables = postProcessVolume->Settings.WeightedBlendables.Array;

				// remove!
				for (int32 i = blendables.Num() - 1; i >= 0; --i)
				{
					if (blendables[i].Object == RadialBlurMaterialInstance)
					{
						blendables.RemoveAt(i);

						static IConsoleVariable* CVarAntiAliasing = IConsoleManager::Get().FindConsoleVariable(TEXT("r.AntiAliasingMethod"));
						if (CVarAntiAliasing)
						{
							CVarAntiAliasing->Set(4, ECVF_SetByCode);
							UE_LOG(LogTemp, Warning, TEXT("AntiAliasing set to None"));
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Failed to find r.AntiAliasingMethod"));
						}
					}
				}

			}
		}
	}
}
