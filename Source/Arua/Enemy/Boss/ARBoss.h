// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/ARMonsterBase.h"
#include "ARBoss.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARBoss : public AARMonsterBase
{
	GENERATED_BODY()
	
public:
	AARBoss();
	
	//추후에 interface로 리팩토링 예정
	void AttackFireBreathSwipe();
	void AttackPawLeft();
	void ComboAttackPawLeft_TailRight();

	//getter, setter
	FORCEINLINE float GetBossAttackRange() { return BossAttackRange; }
	FORCEINLINE float GetBossTurnSpeed() { return BossTurnSpeed; }

	FORCEINLINE float GetAttackFireBreathSwipeTime() { return AttackFireBreathSwipeTime; }
	FORCEINLINE float GetAttackPawLeftTime() { return AttackPawLeftTime; }
	FORCEINLINE float GetComboAttackPawLeft_TailRightTime() { return ComboAttackPawLeft_TailRightTime; }


	virtual void BeginPlay() override;

	//GAS 관련 함수
	virtual void PossessedBy(AController* NewController) override;

	//GA 관련 변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	//Montage 관련 변수
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackFireBreathSwipe;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackPawLeft;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageComboAttackPawLeft_TailRight;

	//Decorator, Task 관련 변수

	//반지름 기준 (기본 크기 50cm, 0.5m)
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float BossAttackRange = 50.0 * 11.0f;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float BossTurnSpeed = 2.0f;


	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float AttackFireBreathSwipeTime = 4.0f;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float AttackPawLeftTime = 1.8f;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float ComboAttackPawLeft_TailRightTime = 3.1f;

};
