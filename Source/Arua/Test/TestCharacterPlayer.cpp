// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "AttributeSet/PlayerAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ATestCharacterPlayer::ATestCharacterPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));

	AttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* ATestCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ATestCharacterPlayer::FinishLockOn()
{
	// 캐릭터가 이동 방향으로 자동 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 카메라 옵션 초기화
	SpringArm->SocketOffset = FVector(0.f, 0.f, 0.f);
	SpringArm->bUsePawnControlRotation = true;
}

void ATestCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ATestCharacterPlayer::LockOnToggle);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ATestCharacterPlayer::TestAttack);
	//EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ATestCharacterPlayer::Roll);
}

void ATestCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ASC->InitAbilityActorInfo(this, this);
}

void ATestCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Ability : StartingAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability, 1, INDEX_NONE, this);
		ASC->GiveAbility(AbilitySpec);
	}
}

void ATestCharacterPlayer::ApplyDamageToTarget(AActor* TargetActor, float BaseDamage, float DamageMultiplier, TSubclassOf<UGameplayEffect> DamageEffectClass)
{
	if (!TargetActor || !DamageEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(
		DamageEffectClass, 1, EffectContext);

	if (SpecHandle.IsValid())
	{
		// SetByCaller로 데미지 값 전달
		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Data.Damage.Base")), BaseDamage);

		SpecHandle.Data->SetSetByCallerMagnitude(
			FGameplayTag::RequestGameplayTag(FName("Data.Damage.Multiplier")), DamageMultiplier);

		ASC->ApplyGameplayEffectSpecToTarget(
			*SpecHandle.Data.Get(), TargetASC);
	}
}

void ATestCharacterPlayer::LockOnToggle(const FInputActionValue& Value)
{
	// Tag로 어빌리티를 찾아서 활성화
	// InputTag나 AbilityTag를 사용할 수 있음
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_LockOn);

	if (!ASC)
	{
		return;
	}

	if (ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		FinishLockOn();
		ASC->CancelAbilities(&TagContainer);
	}

	else
	{
		// 캐릭터가 이동 방향으로 자동 회전하지 않도록 설정
		GetCharacterMovement()->bOrientRotationToMovement = false;
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void ATestCharacterPlayer::Roll(const FInputActionValue& Value)
{
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_Roll);


	if (!ASC)
	{
		return;
	}

	if (!ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void ATestCharacterPlayer::TestAttack(const FInputActionValue& Value)
{
	if (DamageEffect == nullptr)
	{
		return;
	}

	ApplyDamageToTarget(this, 0, 1.0f, DamageEffect);
}
