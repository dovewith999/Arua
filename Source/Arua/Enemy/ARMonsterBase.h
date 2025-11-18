// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterBase.h"
#include "Interface/ARHitReactableInterface.h"
#include "ARMonsterBase.generated.h"

/**
 * 몬스터들의 부모가 될 클래스
 * 몬스터들이 공통으로 가지는 기능을 정의한다.
 * 작성자 : 임희섭
 * 작성일 : 25/11/05
 */
UCLASS()
class ARUA_API AARMonsterBase : public AARCharacterBase, public IARHitReactableInterface
{
	GENERATED_BODY()

public:
	AARMonsterBase();

public:
	void SetTargetLockWidget(bool InShow);
	FORCEINLINE class UBossViewModel* GetViewModel() const { return VM; }

	virtual void IsSensed(bool InIsSensed);
	virtual void IsInNest(bool InIsInNest);

#pragma region // IARHitReactableInterface 구현 부분
	// Inherited via IARHitReactableInterface
	virtual void OnHitByAttack_Implementation(const FHitResult& HitResult, AActor* InInstigator) override;
#pragma endregion

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;

protected:
	// 타겟으로 지정됐을 때 띄울 3D 위젯 - 25/11/05 임희섭
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI|Target", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UARWidgetComponent> TargetLock;

	// AttributeSet 생성 // 25/11/06 임희섭
	//UPROPERTY()
	//TObjectPtr<class UMonsterAttributeSet> AttributeSet;

	UPROPERTY()
	TObjectPtr<class UBossViewModel> VM;

	// 몬스터가 가진 드롭 아이템 목록(사망 시, 드롭)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<TSubclassOf<class AItemPickupActor>> DropItems;
};
