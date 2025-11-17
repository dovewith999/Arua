// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Interact/ARInteractComponent.h"
#include "Character/ARCharacterPlayer.h"
#include "Components/ShapeComponent.h"
#include "Controller/AruaPlayerController.h"
#include "UI/Model/InteractObjectData.h"
#include "UI/ViewModel/ARInteractObjectViewMode.h"

// Sets default values for this component's properties
UARInteractComponent::UARInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UARInteractComponent::SetInteractCollision(UShapeComponent* InCollision, FString InName)
{
	if (InCollision == nullptr)
	{
		return;
	}

	InteractionCollision = InCollision;

	ObjectName = InName;

	UE_LOG(LogTemp, Log, TEXT("Object Initialize"));

	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &UARInteractComponent::OnBeginOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &UARInteractComponent::OnEndOverlap);
}

void UARInteractComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	if (AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(OtherActor))
	{
		if (AAruaPlayerController* PC = Cast<AAruaPlayerController>(Player->GetController()))
		{
			PC->CanInteract(true, ObjectName);
		}
	}
}

void UARInteractComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AARCharacterPlayer* Player = Cast<AARCharacterPlayer>(OtherActor))
	{
		if (AAruaPlayerController* PC = Cast<AAruaPlayerController>(Player->GetController()))
		{
			PC->CanInteract(false);
		}
	}
}
