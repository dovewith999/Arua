// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Tag/AruaGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/CharacterMovementComponent.h"

ATestCharacterPlayer::ATestCharacterPlayer()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
}

UAbilitySystemComponent* ATestCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void ATestCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ATestCharacterPlayer::LockOnToggle);
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

	if (ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		// 캐릭터가 이동 방향으로 자동 회전하도록 설정
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// 컨트롤러의 Yaw 회전을 캐릭터에 적용하도록 (카메라 방향을 향하지 않도록)
		bUseControllerRotationYaw = false;

		if (!ASC)
		{
			return;
		}

		ASC->CancelAbilities(&TagContainer);
	}

	else
	{
		// 캐릭터가 이동 방향으로 자동 회전하지 않도록 설정
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// 컨트롤러의 Yaw 회전을 캐릭터에 적용 (카메라 방향을 향하도록)
		bUseControllerRotationYaw = true;

		if (!ASC)
		{
			return;
		}

		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}