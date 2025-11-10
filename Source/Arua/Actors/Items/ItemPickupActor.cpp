// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Items/ItemPickupActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DataAssets/Item/DA_ItemDefinition.h"
#include "Components/Inventory/InventoryComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AItemPickupActor::AItemPickupActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트 설정
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

}

void AItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void AItemPickupActor::Interact(APawn* Interactor)
{

}

void AItemPickupActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
