// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickupActor.generated.h"

/*
* 월드에 배치되는 아이템 픽업 액터
* 플레이어가 상호작용하면 인벤토리에 아이템을 추가하고 자기 자신을 제거한다.
*/

UCLASS()
class ARUA_API AItemPickupActor : public AActor
{
	GENERATED_BODY()

public:
	AItemPickupActor();

protected:
	virtual void BeginPlay() override;

	/** 플레이어와 상호작용하여 아이템을 인벤토리에 추가한다.
	 * @param Interactor 상호작용하는 캐릭터
	 */
	UFUNCTION(BlueprintCallable, Category = "ItemActor")
	void Interact(APawn* Interactor);

	/** 충돌 시작 시 호출되어 플레이어가 들어왔는지 확인한다. */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	//// 아이템 메시
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemActor")
	//TObjectPtr<class UStaticMeshComponent> ItemMesh;

	//// 상호작용 영역
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemActor")
	//TObjectPtr<class UBoxComponent> InteractionVolume;

	//// 아이템 정의 (아이템 데이터 에셋)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemActor")
	//TObjectPtr<class UItemDefinition> ItemDefinition;

	// 수량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemActor")
	int32 Quantity;
};
