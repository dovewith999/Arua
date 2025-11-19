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
	virtual void Tick(float DeltaTime) override;

	
	//추후에 interface로 리팩토링 예정
	void AttackFireBreathSwipe();
	void AttackPawLeft();
	void ComboAttackPawLeft_TailRight();
	void AttackShootTornado();
	void AttackStump();
	void TurnLeft();
	void TurnRight();


	//getter, setter
	FORCEINLINE float GetAttackFireBreathSwipeTime() { return AttackFireBreathSwipeTime; }
	FORCEINLINE float GetAttackPawLeftTime() { return AttackPawLeftTime; }
	FORCEINLINE float GetComboAttackPawLeft_TailRightTime() { return ComboAttackPawLeft_TailRightTime; }
	FORCEINLINE float GetBossTurnSpeed() { return BossTurnSpeed; }
	FORCEINLINE float GetBossNestRange() { return BossNestRange; }
	FORCEINLINE float GetBossSenseRange() { return BossSenseRange; }
	FORCEINLINE float GetBossAttackRange() const { return BossAttackRange; }

	virtual void BeginPlay() override;



	//GAS 관련 함수
	virtual void PossessedBy(AController* NewController) override;

protected:
	//Decorator, Task 관련 변수
	
	//둥지 설정용 보스 위치
	UPROPERTY()
	FVector NestLocation;


	// 둥지 반경
	UPROPERTY(EditAnywhere, Category = "Nest")
	float BossNestRange = 50.0f * 30 * 2;

	// 감지 반경 
	UPROPERTY(EditAnywhere, Category = "Sense")
	float BossSenseRange = 50.0f * 30 * 2;

	// 공격 반경
	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float BossAttackRange = 50.0f * 8 * 2;
	//반지름 기준 (기본 크기 50cm, 0.5m)
	//기본 공격 범위 :  50*5*2 로 할듯.
	//범위 공격 : 50.0f * 8 * 2;

	// 회전 속도
	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float BossTurnSpeed = 20.0f;

	


	UFUNCTION(BlueprintCallable)
	void SpawnBossActor(TSubclassOf<AActor> ActorToSpawn, float Distance, float SetZAxis);

	//토네이도 쏘는 이벤트
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnTornado();

	virtual void IsSensed(bool InIsSensed) override;

	//GA 관련 변수
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	//Montage 관련 변수
	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackFireBreathSwipe;

	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackPawLeft;

	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageComboAttackPawLeft_TailRight;

	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackShootTornado;

	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageAttackStump;


public:
	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageTurnLeft;

	UPROPERTY(EditAnywhere, Category = BossMontage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> MontageTurnRight;


protected:

	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float AttackFireBreathSwipeTime = 4.0f;

	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float AttackPawLeftTime = 1.8f;

	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float ComboAttackPawLeft_TailRightTime = 3.1f;

	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float AttackShootTornadoTime = 2.95f;

	UPROPERTY(EditAnywhere, Category = BossMontageTime, meta = (AllowPrivateAccess = "true"))
	float AttackStumpTime = 2.46f;


	//소환물 관련 변수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TSubclassOf<AActor> SpawnExplode;

private:
	UPROPERTY()
	TObjectPtr<class UBossData> Model;


};
