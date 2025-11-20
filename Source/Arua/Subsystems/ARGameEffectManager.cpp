// Fill out your copyright notice in the Description page of Project Settings.


#include "ARGameEffectManager.h"
#include "Materials/MaterialInstance.h"
#include "Engine/PostProcessVolume.h"
#include "Engine/BlendableInterface.h"
#include "EngineUtils.h"

UARGameEffectManager::UARGameEffectManager()
{
}

void UARGameEffectManager::StartBlur()
{
	if (RadialBlurMaterialInstance)
	{
		for (TActorIterator<APostProcessVolume> psIt(GetWorld()); psIt; ++psIt)
		{
			APostProcessVolume* postProcessVolume = *psIt;
			if (postProcessVolume)
			{
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
					}
				}
			}
		}
	}
}
