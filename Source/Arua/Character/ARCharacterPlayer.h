// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterBase.h"
#include "InputActionValue.h"
#include "ARCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARUA_API AARCharacterPlayer : public AARCharacterBase
{
	GENERATED_BODY()

public:
	AARCharacterPlayer();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool GetRunState() { return bIsRunning; }

	FORCEINLINE bool GetWalkState() { return bIsWalking; }

	FORCEINLINE class UARComboActionData* GetComboActionData() const { return ComboActionData; }

	virtual class UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }

	virtual void PossessedBy(AController* NewController) override;

public:
	void FinishLockOn();
	FORCEINLINE class UAnimMontage* GetRollMontage() const { return RollActionMontage; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	// 카메라 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camear, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	// 입력 관련 섹션
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LockOnAction;

	// NPC 상호작용 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRunning : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsWalking : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRolling : 1;

	void Move(const FInputActionValue& Value);
	void NotMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void RunTriggered(const FInputActionValue& Value);
	void RunComplete(const FInputActionValue& Value);
	virtual void Roll(const FInputActionValue& Value);
	void LockOnToggle(const FInputActionValue& Value);
	void RollCompleted();


	// NPC 상호작용 입력 콜백 함수
	void NPCInteraction(const FInputActionValue& Value);

	// 죽음 처리 함수
	virtual void SetDead() override;

	// 애니메이션 섹션
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Animation)
	TObjectPtr<class UAnimMontage> RollActionMontage;

	FTimerHandle RollAnimTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 무기 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;

	// ASC 어빌리티 섹션
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GAS)
	TMap<int32, TSubclassOf<class UGameplayAbility>> StartInputAbilities;

protected:
	void SetupGASInputComponent();
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	// 콤보 액션 데이터 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UARComboActionData> ComboActionData;

	// 퀘스트 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<class UQuestComponent> QuestComponent;


};
