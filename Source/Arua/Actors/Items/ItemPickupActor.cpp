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

	// 아이템 메시 컴포넌트 초기화
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);

	// 충돌 컴포넌트 초기화
	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
	InteractionCollision->SetupAttachment(RootComponent);
	InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractionCollision->SetCollisionObjectType(ECC_WorldDynamic);
	InteractionCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 충돌 오버랩 함수 바인딩
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemPickupActor::OnBeginOverlap);

	// 기본 수량 초기화
	Quantity = 1;
}

void AItemPickupActor::BeginPlay()
{
	Super::BeginPlay();
}

void AItemPickupActor::Interact(APawn* Interactor)
{
	if (!Interactor || !ItemDefinition) return;

	// 플레이어의 인벤토리 컴포넌트 가져오기
	UInventoryComponent* InventoryComp = Interactor->FindComponentByClass<UInventoryComponent>();
	if (InventoryComp)
	{
		// 아이템 추가
		int32 Added = InventoryComp->AddItem(ItemDefinition, Quantity);
		//if (Added > 0)
		//{
		//	// 성공적으로 추가되면 아이템 액터 삭제
		//	Destroy();
		//}
		Destroy();
	}
}

void AItemPickupActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* Pawn = Cast<APawn>(OtherActor))
	{
		// 플레이어에게 위젯 띄우고 픽업 입력 액션 할당하기?

		// 오버랩 되었을 때 자동으로 인터랙트하지 않고, 플레이어 입력에 의해 호출되도록 하려면 인터랙트 함수를 호출하지 않는다.
		// 예: 캐릭터가 E 키를 눌렀을 때 주변 아이템을 찾도록 구현 가능.
	}
}
