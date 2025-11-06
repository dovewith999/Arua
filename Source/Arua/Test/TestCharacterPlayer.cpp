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
