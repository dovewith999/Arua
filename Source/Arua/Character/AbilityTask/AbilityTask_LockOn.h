// AbilityTask_LockOn.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_LockOn.generated.h"

/**
 * AbilityTask: Owner가 Target을 계속 바라보도록 회전
 */
UCLASS()
class ARUA_API UAbilityTask_LockOn : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * Target을 바라보는 Task 생성
	 * @param OwningAbility - 이 Task를 소유한 Ability
	 * @param TargetActor - 바라볼 타겟
	 * @param RotationSpeed - 회전 속도 (0 = 즉시 회전)
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_LockOn* CreateLockOnTask(UGameplayAbility* OwningAbility, AActor* TargetActor, float RotationSpeed = 10.0);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<AActor> Target;

	float InterpSpeed;
};