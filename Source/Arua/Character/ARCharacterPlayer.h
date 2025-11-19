// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARCharacterBase.h"
#include "InputActionValue.h"
#include "Character/Weapon/ARWeaponBase.h"
#include "Interface/ARHitReactableInterface.h"
#include "ARCharacterPlayer.generated.h"

// 방향을 알기 위한 enum
// 작성자 : 임희섭
// 작성일 : 25/11/12
UENUM(BlueprintType)
enum class EInputDirection : uint8
{
	Front,
	Back,
	Left,
	Right
};

// Ability Input Binding을 위한 구조체 
// 작성자 : 임희섭
// 작성일 : 25/11/13
USTRUCT(BlueprintType)
struct FAbilityInputMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> InputAction;
};

/**
 * 
 */
UCLASS()
class ARUA_API AARCharacterPlayer : public AARCharacterBase, public IARHitReactableInterface
{
	GENERATED_BODY()

public:
	AARCharacterPlayer();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool GetRunState() { return bIsRunning; }

	FORCEINLINE bool GetWalkState() { return bIsWalking; }

	FORCEINLINE class UARComboActionData* GetComboActionData() const { return ComboActionData; }
	FORCEINLINE bool GetIsWeaponChanged() { return bIsWeaponChanged; }
	FORCEINLINE void SetIsWeaponChanged(bool change) { bIsWeaponChanged = change; }
	FORCEINLINE FGameplayTag GetWeaponTag() { return CurrentWeapon->WeaponTag; }
	FORCEINLINE AARWeaponBase* GetCurrentWeapon() { return CurrentWeapon; }

	FORCEINLINE class UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE class UQuestComponent* GetQuestComponent() const { return QuestComponent; }

	virtual class UAnimMontage* GetComboActionMontage() const { return ComboActionMontage; }

	virtual void PossessedBy(AController* NewController) override;

	void MontageEnded();

public:
	void BeginLockOn();
	void FinishLockOn();
	void SetInputDirection(); // 카메라 기준 입력 방향을 계산하기 위한 함수 - 25/11/12 임희섭
	FName GetLockOnDodgeMontageSection() const;
	EInputDirection GetDodgeDirection() const;


	UFUNCTION(BlueprintCallable)
	AARWeaponBase* WeaponChange(AARWeaponBase* NewWeapon);

#pragma region Hit React Interface
	// Inherited via IARHitReactableInterface
	virtual void OnHitByAttack_Implementation(const FHitResult& HitResult, AActor* InInstigator) override;
#pragma endregion

	FORCEINLINE class UAnimMontage* GetRollMontage() const { return RollActionMontage; }
	FORCEINLINE class UAnimMontage* GetLockOnDodgeMontage() const { return LockOnDodgeActionMontage; }
	class UAnimMontage* GetSkillMontage(const struct FGameplayTag& InTag) const;
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }

	// Todo: 이름 변경할 필요 있음.
	// 애니메이션에서 Weapon 관련 GA 이벤트 등록을 완료한 후에 호출하는 함수.
	void EquipStartWeapon();



	// 카메라 섹션
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Minimap", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> MinimapSpringArm;

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

	// 인벤토리 상호작용 입력 액션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InventoryAction;

#pragma region Skill Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillWhirlwindAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkillChargeAttackAction;
#pragma endregion

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
	void WeaponChangeTest();

	// NPC 상호작용 입력 콜백 함수
	void NPCInteraction(const FInputActionValue& Value);

	// 인벤토리 토글 입력 콜백 함수
	void ToggleInventory();

	// 죽음 처리 함수
	virtual void SetDead() override;

	// 애니메이션 섹션
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Animation)
	TObjectPtr<class UAnimMontage> RollActionMontage;

	FTimerHandle RollAnimTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> LockOnDodgeActionMontage;

	// 콤보 공격 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	// 히트 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hit Reaction")
	TObjectPtr<class UAnimMontage> HitReactMontage;

#pragma region Skill Section
	// 스킬 - 휠윈드 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Whirlwind")
	TObjectPtr<class UAnimMontage> SkillWhirlwindMontage;
#pragma endregion


// ASC 어빌리티 섹션
protected:
	void SetupGASInputComponent();
	void GASInputHoldStart(int32 InputId);
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);

	// 콤보 액션 데이터 
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UARComboActionData> ComboActionData;

	// 퀘스트 섹션
protected:
	// 퀘스트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<class UQuestComponent> QuestComponent;

	// 인벤토리 섹션
protected:
	UFUNCTION()
	void ResetInventoryToggleCooldown();

	// 인벤토리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	// 인벤토리 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	// 인벤토리 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UInventoryWidget> InventoryWidgetInstance;

	// 인벤토리 열기/닫기 타이머
	FTimerHandle InventoryTimerHandle;

	// 인벤토리가 열려있는 상태인지 플래그
	UPROPERTY()
	bool bIsOpenInventory = false;

	// 인벤토리를 열기/닫기 할 수 있는 상태
	UPROPERTY()
	bool bCanToggleInventory = true;

	// 무기 교체 섹션.
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AARWeaponBase> CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AARWeaponBase> WeaponForChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	FGameplayTag WeaponTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Weapon)
	uint8 bIsWeaponChanged : 1;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AARWeaponBase> StartWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WeaponChangeAction;

	void EquipWeapon(class AARWeaponBase* EWeapon, FName SocketName);
	void UnequipWeapon(class AARWeaponBase* EWeapon);
	void UnequipWeaponTest(class AARWeaponBase* EWeapon);

private:
	FVector2D CurrentInputAxis = FVector2D::ZeroVector;			// 방향 입력 값 저장용

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<FAbilityInputMapping> AbilityInputMappings;

	UPROPERTY()
	TMap<TObjectPtr<class UInputAction>, int> InputIds;

	// 몽타주 섹션
protected:
	void PlayAction(FGameplayTag ActionTag);

};
