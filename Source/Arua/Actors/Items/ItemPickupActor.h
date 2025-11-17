// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickupActor.generated.h"

/*
* 월드에 배치되는 아이템 픽업 액터
* 플레이어가 상호작용하면 인벤토리에 아이템을 추가하고 자기 자신을 제거
*/
UCLASS()
class ARUA_API AItemPickupActor : public AActor
{
	GENERATED_BODY()

public:
	AItemPickupActor();

protected:
	virtual void BeginPlay() override;

	// 아이템 상호작용 함수
	/* 플레이어와 상호작용하여 아이템을 인벤토리에 추가 후, 액터 제거 */
	UFUNCTION(BlueprintCallable, Category = "ItemActor")
	void Interact(APawn* Interactor);

	// 충돌 오버랩 함수 (플레이어에게 픽업 메시지 전달을 구현할 경우)
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// 아이템 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemActor")
	TObjectPtr<class UStaticMeshComponent> ItemMesh;

	// 아이템 상호작용 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemActor")
	TObjectPtr<class UBoxComponent> InteractionCollision;

	// 아이템 정의 (아이템 데이터 에셋)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemActor")
	TObjectPtr<class UDA_ItemDefinition> ItemDefinition;

	// 수량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemActor")
	int32 Quantity;


protected:

};
