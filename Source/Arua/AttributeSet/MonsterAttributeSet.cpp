// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/MonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
//#include "AbilitySystemGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/AruaPlayerController.h"

UMonsterAttributeSet::UMonsterAttributeSet()
{
    MaxHealth.SetBaseValue(50.f);
    MaxHealth.SetCurrentValue(50.f);

    Health.SetBaseValue(50.f);
    Health.SetCurrentValue(50.f);

    Attack.SetBaseValue(10.f);
    Attack.SetCurrentValue(10.f);
}

void UMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data); 

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

        if (GetHealth() <= 0.f)
        {
            if (AAruaPlayerController* PC = Cast<AAruaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
            {
                PC->TargetOff();
            }

            //// 게임플레이 이벤트 데이터를 만들어, 충돌한 대상을 저장
            //FGameplayEventData Payload;
            //Payload.Instigator = OwnerActor;
            //Payload.Target = OwnerActor;
            //Payload.EventMagnitude = 1.0f;

            //// 자신에게 근접 공격 이벤트 전달
            //UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);
        }
    }
}
