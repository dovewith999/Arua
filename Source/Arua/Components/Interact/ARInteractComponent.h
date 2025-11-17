// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AR_NPCInteractionInterface.h"
#include "ARInteractComponent.generated.h"

/*
* 플레이어랑 상호작용 할 수 있는 오브젝트가 가지는 컴포넌트
* 작성자 : 임희섭
* 작성일 : 25/11/17
*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARUA_API UARInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UARInteractComponent();

public:
	void SetInteractCollision(class UShapeComponent* InCollision, FString InName);	

private:
	// 충돌 오버랩 함수 (플레이어에게 픽업 메시지 전달을 구현할 경우)
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

protected:
	UPROPERTY()
	TObjectPtr<class UShapeComponent> InteractionCollision;

	UPROPERTY()
	FString ObjectName;

	UPROPERTY()
	TObjectPtr<class UARInteractObjectViewModel> InteractViewModel;
};
