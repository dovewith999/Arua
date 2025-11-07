// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ARCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "TimerManager.h"
#include "Player/ARPlayerState.h"
#include "AbilitySystemComponent.h"
#include "GA/ARGA_Attack.h"
#include "Character/ARComboActionData.h"

#include "Components/Quest/QuestComponent.h"
#include "Interface/AR_NPCInteractionInterface.h"

#include "Tag/AruaGameplayTags.h"

#include "AttributeSet/PlayerAttributeSet.h"
#include "Controller/AruaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DamageLibrary.h"

AARCharacterPlayer::AARCharacterPlayer()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(TEXT("/Game/MercenaryWarrior/Meshes/SK_MercenaryWarrior_WithoutHelmet.SK_MercenaryWarrior_WithoutHelmet"));
	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Animation/Player/ABP_ARSwordCharacter.ABP_ARSwordCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(
		GetMesh(),
		FName("hand_rSocket")
	);

	// 퀘스트 컴포넌트 CDO 초기화
	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Game/Input/IMC_Default.IMC_Default"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Game/Input/IA_Look.IA_Look"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Game/Input/IA_Move.IA_Move"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRunRef(TEXT("/Game/Input/IA_Run.IA_Run"));
	if (nullptr != InputActionRunRef.Object)
	{
		RunAction = InputActionRunRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRollRef(TEXT("/Game/Input/IA_Roll.IA_Roll"));
	if (nullptr != InputActionRollRef.Object)
	{
		RollAction = InputActionRollRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Game/Input/IA_Attack.IA_Attack"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WeaponRef(TEXT("/Game/GreatSword/GreatSword/Weapon/GreatSword_01.GreatSword_01"));
	if (nullptr != WeaponRef.Object)
	{
		Weapon->SetStaticMesh(WeaponRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/Animation/Player/AM_SwordComboAttack.AM_SwordComboAttack"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/Animation/Player/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UARComboActionData> ComboActionDataRef(TEXT("/Script/Arua.ARComboActionData'/Game/GameData/ARA_ComboAttack.ARA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

}

void AARCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	bIsRunning = false;
	bIsWalking = false;
	bIsRolling = false;
}



void AARCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AARCharacterPlayer::NotMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::RunTriggered);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AARCharacterPlayer::RunComplete);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &AARCharacterPlayer::Roll);
	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &AARCharacterPlayer::LockOnToggle);
	EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::NPCInteraction);
	//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::GASInputPressed, 0);

	SetupGASInputComponent();

}

void AARCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AARPlayerState* PS = GetPlayerState<AARPlayerState>();
	if (PS)
	{
		ASC = PS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(PS, this);

		AttributeSet = PS->GetAttributeSet();

		int InputId = 0;
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			StartSpec.InputID = InputId++;
			ASC->GiveAbility(StartSpec);
		}

		SetupGASInputComponent();
	}
}

void AARCharacterPlayer::BeginLockOn()
{
	// 캐릭터가 이동 방향으로 자동 회전하지 않도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AARCharacterPlayer::FinishLockOn()
{
	// 캐릭터가 이동 방향으로 자동 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AARCharacterPlayer::Move(const FInputActionValue& Value)
{
	//if (bIsRolling)
	//    return;

	bIsWalking = true;
	FVector2D MovementVector = Value.Get<FVector2D>();

	FRotator Rotation;
	if (ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		// LockOn일 때는 SprintArm의 Forward를 기준으로 이동 방향을 정하도록 설정 25/11/06 임희섭
		Rotation = SpringArm->GetComponentRotation();
	}

	else
	{
		Rotation = Controller->GetControlRotation();
	}

	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);

}

void AARCharacterPlayer::NotMove(const FInputActionValue& Value)
{
	bIsWalking = false;
}

void AARCharacterPlayer::Look(const FInputActionValue& Value)
{
	if (ASC == nullptr || ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AARCharacterPlayer::RunTriggered(const FInputActionValue& Value)
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0;
}

void AARCharacterPlayer::RunComplete(const FInputActionValue& Value)
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0;
}

void AARCharacterPlayer::Roll(const FInputActionValue& Value)
{
	//if (bIsRolling)
	//	return;

	//bIsRolling = true;

	//PlayAnimMontage(RollActionMontage);

	//GetWorld()->GetTimerManager().SetTimer(
	//	RollAnimTimer,
	//	this,
	//	&AARCharacterPlayer::RollCompleted ,
	//	RollActionMontage->GetPlayLength(),
	//	false
	//);

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

void AARCharacterPlayer::LockOnToggle(const FInputActionValue& Value)
{
	// TODO 나중에 지워야함 죽음 처리용 즉사 코드
	// UDamageLibrary::ApplyDamage(ASC, this, AttributeSet->GetMaxHealth());

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
		BeginLockOn();
		ASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void AARCharacterPlayer::RollCompleted()
{
	bIsRolling = false;
}

void AARCharacterPlayer::NPCInteraction(const FInputActionValue& Value)
{
	FHitResult HitResult;

	// 카메라 위치 기준으로 선형 트레이싱을 통해 상호작용 가능 NPC 객체 감지
	FVector Start = SpringArm->GetComponentLocation();
	FVector End = Start + SpringArm->GetForwardVector() * 300.f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		if (HitResult.bBlockingHit)
		{
			// 인터렉션 인터페이스로 캐스팅하여 NPC 상호작용 시작
			IAR_NPCInteractionInterface* NPCInteractionInterface = Cast<IAR_NPCInteractionInterface>(HitResult.GetActor());
			NPCInteractionInterface->PlayInteraction(this);
		}
	}
}

void AARCharacterPlayer::SetDead()
{
	Super::SetDead();

	// 후에 있을지 모르는 플레이어 죽음 이후 상태 구현.
}

void AARCharacterPlayer::SetupGASInputComponent()
{

	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::GASInputPressed, 0);
	}
}

void AARCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AARCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

