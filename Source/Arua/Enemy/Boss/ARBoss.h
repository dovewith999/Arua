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
	void TurnLeft();
	void TurnRight();


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
public:
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageTurnLeft;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageTurnRight;
protected:
	//Decorator, Task 관련 변수

	//반지름 기준 (기본 크기 50cm, 0.5m)
	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float BossAttackRange = 50.0 * 11.0f;/*현재 scale 2배 해놔서, 공격 범위도 2배임->아직 안함*/

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float BossTurnSpeed = 10.0f;


	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float AttackFireBreathSwipeTime = 4.0f;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float AttackPawLeftTime = 1.8f;

	UPROPERTY(EditAnywhere, Category = CharacterControl, meta = (AllowPrivateAccess = "true"))
	float ComboAttackPawLeft_TailRightTime = 3.1f;

};
