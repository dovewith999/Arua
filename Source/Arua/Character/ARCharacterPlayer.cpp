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
#include "Components/Inventory/InventoryComponent.h"
#include "Interface/AR_NPCInteractionInterface.h"

#include "UI/Inventory/InventoryWidget.h"

#include "Tag/AruaGameplayTags.h"

#include "AttributeSet/PlayerAttributeSet.h"
#include "Controller/AruaPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DamageLibrary.h"

#include "Weapon/ARWeaponBase.h"
#include "Kismet/KismetMathLibrary.h"

#include "Animation/ARCharacterAnimInstance.h"

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

	MinimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	MinimapSpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// 퀘스트 컴포넌트 CDO 초기화
	QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));

	// 인벤토리 컴포넌트 CDO 초기화
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

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

	static ConstructorHelpers::FObjectFinder<UInputAction> WeaponChangeActionRef(TEXT("/Game/Input/IA_Weapon.IA_Weapon"));
	if (nullptr != WeaponChangeActionRef.Object)
	{
		WeaponChangeAction = WeaponChangeActionRef.Object;
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

	//if (StartWeaponClass)
	//{
	//	CurrentWeapon = GetWorld()->SpawnActor<AARWeaponBase>(StartWeaponClass);

	//	if (CurrentWeapon)
	//	{
	//		EquipWeapon(CurrentWeapon, CurrentWeapon->Socket);
	//	}
	//}

	/*WeaponType = EWeaponType::None;*/
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
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::ToggleInventory);
	EnhancedInputComponent->BindAction(WeaponChangeAction, ETriggerEvent::Started, this, &AARCharacterPlayer::WeaponChangeTest);
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
		//for (const auto& StartAbility : StartAbilities)
		//{
		//	FGameplayAbilitySpec StartSpec(StartAbility);
		//	StartSpec.InputID = InputId++;
		//	ASC->GiveAbility(StartSpec);
		//}

		for (const auto& Ability : AbilityInputMappings)
		{
			FGameplayAbilitySpec StartSpec(Ability.AbilityClass);
			StartSpec.InputID = InputId++;
			ASC->GiveAbility(StartSpec);

			if (Ability.InputAction != nullptr)
			{
				InputIds.Add(Ability.InputAction, StartSpec.InputID);
			}
		}

		SetupGASInputComponent();
	}
}

void AARCharacterPlayer::BeginLockOn()
{
	// 캐릭터가 이동 방향으로 자동 회전하지 않도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_LockOn);
	ASC->TryActivateAbilitiesByTag(TagContainer);
}

void AARCharacterPlayer::FinishLockOn()
{
	// 캐릭터가 이동 방향으로 자동 회전하도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(AruaGamePlayTags::Ability_LockOn);
	ASC->CancelAbilities(&TagContainer);
}

void AARCharacterPlayer::SetInputDirection()
{
	if (ASC == nullptr || ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		return;
	}

	FVector CameraForwardDir = FVector(Camera->GetForwardVector().X, Camera->GetForwardVector().Y, 0.f) * CurrentInputAxis.Y;
	FVector CameraRightDir = FVector(Camera->GetRightVector().X, Camera->GetRightVector().Y, 0.f) * CurrentInputAxis.X;
	FVector TotalDir = CameraForwardDir + CameraRightDir;

	if (TotalDir.SizeSquared() < 0.01f)
	{
		return;
	}

	TotalDir.Normalize();

	// SprintArm 튀는 현상 방지 - 25/11/12 임희섭
	// 1. 회전 전 SpringArm의 월드 회전 저장
	FRotator SavedSpringArmRotation = SpringArm->GetComponentRotation();
	FRotator SavedMinimapSpringArmRotation = MinimapSpringArm->GetComponentRotation();

	FRotator Rot = FRotationMatrix::MakeFromX(TotalDir).Rotator();
	SetActorRotation(Rot);

	// 2. SpringArm을 저장된 회전으로 즉시 복원 (Controller 회전 유지)
	SpringArm->SetWorldRotation(SavedSpringArmRotation);
	MinimapSpringArm->SetWorldRotation(SavedMinimapSpringArmRotation);
}

FName AARCharacterPlayer::GetLockOnDodgeMontageSection() const
{
	EInputDirection Direction = GetDodgeDirection();

	switch (Direction)
	{
	case EInputDirection::Front:
		return FName("Front");
	case EInputDirection::Back:
		return FName("Back");
	case EInputDirection::Left:
		return FName("Left");
	case EInputDirection::Right:
		return FName("Right");
	default:
		return FName("Front");
	}
}

EInputDirection AARCharacterPlayer::GetDodgeDirection() const
{
	// 입력이 거의 없으면 전방
	if (CurrentInputAxis.IsNearlyZero(0.1f))
	{
		return EInputDirection::Front;
	}

	// 절댓값이 더 큰 축 우선
	if (FMath::Abs(CurrentInputAxis.Y) > FMath::Abs(CurrentInputAxis.X))
	{
		return CurrentInputAxis.Y > 0 ? EInputDirection::Front : EInputDirection::Back;
	}
	else
	{
		return CurrentInputAxis.X > 0 ? EInputDirection::Right : EInputDirection::Left;
	}
}

UAnimMontage* AARCharacterPlayer::GetSkillMontage(const FGameplayTag& InTag) const
{
	UAnimMontage* ReturnMontage = nullptr;

	if (InTag == AruaGamePlayTags::Ability_Whirlwind)
	{
		ReturnMontage = SkillWhirlwindMontage;
	}

	return ReturnMontage;
}

void AARCharacterPlayer::Move(const FInputActionValue& Value)
{
	//if (bIsRolling)
	//    return;

	bIsWalking = true;
	FVector2D MovementVector = Value.Get<FVector2D>();
	CurrentInputAxis = MovementVector;

	FRotator Rotation;
	if (ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		// LockOn일 때는 Camera의 Forward를 기준으로 이동 방향을 정하도록 설정 - 25/11/06 임희섭
		Rotation = Camera->GetComponentRotation();
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
	CurrentInputAxis = FVector2D::Zero();
}

void AARCharacterPlayer::Look(const FInputActionValue& Value)
{
	if (ASC == nullptr || ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		return;
	}

	//FVector2D LookAxisVector = Value.Get<FVector2D>();

	//AddControllerYawInput(LookAxisVector.X);
	//AddControllerPitchInput(LookAxisVector.Y);

	// 카메라 Ptich 제한 - 25/11/18 임희섭
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (LookAxisVector.Y != 0.f)
	{

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (PC)
		{
			// 현재 pitch 가져오기
			FRotator ControlRot = PC->GetControlRotation();
			float CurrentPitch = FRotator::NormalizeAxis(ControlRot.Pitch);

			// AddControllerPitchInput는 delta라서, 일단 적용할 값 계산
			float NewPitch = CurrentPitch - LookAxisVector.Y;

			// 원하는 범위로 제한
			NewPitch = FMath::Clamp(NewPitch, -55.f, 10.f);

			// 최종 적용
			ControlRot.Pitch = NewPitch;

			PC->SetControlRotation(ControlRot);
		}
	}
}

void AARCharacterPlayer::RunTriggered(const FInputActionValue& Value)
{
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = 1000.0;
}

void AARCharacterPlayer::RunComplete(const FInputActionValue& Value)
{
	bIsRunning = false;
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.0;
}

void AARCharacterPlayer::Roll(const FInputActionValue& Value)
{
	/*if (bIsRolling)
		return;

	bIsRolling = true;

	PlayAnimMontage(RollActionMontage);

	GetWorld()->GetTimerManager().SetTimer(
		RollAnimTimer,
		this,
		&AARCharacterPlayer::RollCompleted ,
		RollActionMontage->GetPlayLength(),
		false
	);*/

	FGameplayTagContainer TagContainer;


	if (!ASC)
	{
		return;
	}

	if (!ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		TagContainer.AddTag(AruaGamePlayTags::Ability_Roll);
		ASC->TryActivateAbilitiesByTag(TagContainer);
		PlayAction(FGameplayTag::RequestGameplayTag("Character.Action.Roll"));
	}

	else
	{
		TagContainer.AddTag(AruaGamePlayTags::Ability_LockOnDodge);
		ASC->TryActivateAbilitiesByTag(TagContainer);
		PlayAction(FGameplayTag::RequestGameplayTag("Character.Action.LockOnSlide"));
	}

}

void AARCharacterPlayer::LockOnToggle(const FInputActionValue& Value)
{
	// Tag로 어빌리티를 찾아서 활성화
	// InputTag나 AbilityTag를 사용할 수 있음


	if (!ASC)
	{
		return;
	}

	if (ASC->HasMatchingGameplayTag(AruaGamePlayTags::Player_State_LockOn))
	{
		FinishLockOn();
	}

	else
	{
		BeginLockOn();

	}
}

void AARCharacterPlayer::RollCompleted()
{
	bIsRolling = false;
}

AARWeaponBase* AARCharacterPlayer::WeaponChange(AARWeaponBase* NewWeapon)
{
	if ((GetCharacterMovement()->Velocity.Size2D()) > KINDA_SMALL_NUMBER || GetIsWeaponChanged())
	{
		return nullptr;
	}

	TObjectPtr<class AARWeaponBase> PreviousWeapon = CurrentWeapon;

	FTimerHandle TimerHandle;
	FTimerHandle AnimTimerHandle;
	FName SectionName;
	if (CurrentWeapon == nullptr)
	{
		CurrentWeapon = NewWeapon;

		if (CurrentWeapon)
		{
			EquipWeapon(CurrentWeapon, CurrentWeapon->Socket);
			/*GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(SwordAnimClass.LoadSynchronous());*/
			//SectionName = TEXT("WeaponEquip")
		}
		return nullptr;
	}
	else
	{
		UnequipWeapon(CurrentWeapon);
		CurrentWeapon = NewWeapon;
		EquipWeapon(CurrentWeapon,CurrentWeapon->Socket);

		return PreviousWeapon;
		//SectionName = TEXT("WeaponUnarm");
	}


	/*float MontageLength = GetMesh()->GetAnimInstance()->Montage_Play(WeaponEquipMontage, 1.0f);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, WeaponEquipMontage);*/
	//GetWorldTimerManager().SetTimer(TimerHandle, []() { /* 아무 작업 안함 */ }, WeaponEquipMontage->GetSectionLength(WeaponEquipMontage->GetSectionIndex(SectionName)), false);

}

void AARCharacterPlayer::WeaponChangeTest()
{
	if ((GetCharacterMovement()->Velocity.Size2D()) > KINDA_SMALL_NUMBER || GetIsWeaponChanged())
	{
		return;
	}

	FTimerHandle TimerHandle;
	FTimerHandle AnimTimerHandle;
	FName SectionName;
	if (CurrentWeapon == nullptr)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AARWeaponBase>(StartWeaponClass);

		if (CurrentWeapon)
		{
			EquipWeapon(CurrentWeapon, TEXT("hand_rSocket"));
			/*GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
			GetMesh()->SetAnimInstanceClass(SwordAnimClass.LoadSynchronous());*/
			//SectionName = TEXT("WeaponEquip")
		}
	}
	else
	{
		UnequipWeapon(CurrentWeapon);
		//SectionName = TEXT("WeaponUnarm");
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *WeaponTag.ToString());

	/*float MontageLength = GetMesh()->GetAnimInstance()->Montage_Play(WeaponEquipMontage, 1.0f);
	GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, WeaponEquipMontage);*/
	//GetWorldTimerManager().SetTimer(TimerHandle, []() { /* 아무 작업 안함 */ }, WeaponEquipMontage->GetSectionLength(WeaponEquipMontage->GetSectionIndex(SectionName)), false);
}

void AARCharacterPlayer::NPCInteraction(const FInputActionValue& Value)
{
	if (!GetWorld()) return;

	// #1: 트레이스 파라미터 설정
	// 캐릭터의 오리진보다 약간 높게 시작 시점 설정
	const float TraceStartHeight = 40.f;

	// 아래로 트레이싱할 거리
	const float TraceDownLength = 150.f;

	// 트레이싱할 박스 반지름
	const FVector BoxHalfExtent(20.f, 20.f, 20.f);

	const FVector ActorLocation = GetActorLocation();

	// 시작은 캐릭터에서 위쪽
	const FVector Start = ActorLocation + FVector(0.f, 0.f, TraceStartHeight);
	// 끝은 아래 방향으로
	const FVector End = ActorLocation - FVector(0.f, 0.f, TraceDownLength);

	// #2: 박스 스윕 설정
	FCollisionShape BoxShape = FCollisionShape::MakeBox(BoxHalfExtent);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(NPCInteractionTrace), false, this);
	Params.bReturnPhysicalMaterial = false;
	Params.AddIgnoredActor(this); // 자기 자신 무시

	TArray<FHitResult> HitResults;

	const bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Start,
		End,
		GetActorForwardVector().ToOrientationQuat(), // 플레이어 전방 기준으로 회전
		ECC_Visibility,
		BoxShape,
		Params
	);

	// 디버그 시각화 (원하면 사용)

	DrawDebugBox(GetWorld(), Start, BoxHalfExtent, FQuat::Identity, FColor::Green, false, 1.f);
	DrawDebugBox(GetWorld(), End, BoxHalfExtent, FQuat::Identity, FColor::Red, false, 1.f);
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 1.f, 0, 1.f);

	// 충돌 물체가 없으면 종료
	if (!bHit || HitResults.Num() == 0) return;

	// #3: 같은 액터가 여러 컴포넌트로 여러 번 맞는 경우를 피하기 위해 Set 사용
	TSet<AActor*> ProcessedActors;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || ProcessedActors.Contains(HitActor))
		{
			continue;
		}

		// 중복 방지
		ProcessedActors.Add(HitActor);

		// #4: 인터렉션 인터페이스 체크 후 상호작용 실행
		if (IAR_NPCInteractionInterface* NPCInteractionInterface = Cast<IAR_NPCInteractionInterface>(HitActor))
		{
			NPCInteractionInterface->PlayInteraction(this);
			return; // 처음으로 상호작용을 성공하면 종료
		}
	}
}

void AARCharacterPlayer::ToggleInventory()
{
	// 쿨타임 중이면 토글 차단
	if (!bCanToggleInventory) return;

	// 이번에 토글을 수행하므로, 쿨타임 시작
	bCanToggleInventory = false;

	// 인벤토리 토글 플래그 설정
	GetWorld()->GetTimerManager().SetTimer(
		InventoryTimerHandle,
		this,
		&AARCharacterPlayer::ResetInventoryToggleCooldown,
		0.5f,
		false
	);

	// 인벤토리 열기
	if (!InventoryWidgetInstance && !bIsOpenInventory)
	{
		if (!InventoryWidgetClass) return;

		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC) return;

		if (!InventoryWidgetInstance)
		{
			// 인벤토리 위젯 인스턴스 생성 및 초기화
			InventoryWidgetInstance = CreateWidget<UInventoryWidget>(PC, InventoryWidgetClass);
			if (!InventoryWidgetInstance) return;

			InventoryWidgetInstance->AddToViewport();
			InventoryWidgetInstance->InitializeInventory(InventoryComponent);
		}

		// 인벤토리 가시화
		InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);

		// 입력 모드 및 커서 설정
		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(InventoryWidgetInstance->TakeWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		Mode.SetHideCursorDuringCapture(false);
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = true;

		bIsOpenInventory = true;
	}
	// 인벤토리 닫기
	else if (InventoryWidgetInstance && bIsOpenInventory)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC) return;

		if (InventoryWidgetInstance)
		{
			InventoryWidgetInstance->RemoveFromParent();
			InventoryWidgetInstance = nullptr;
		}

		// 입력 모드 게임 모드로 복귀
		FInputModeGameOnly Mode;
		PC->SetInputMode(Mode);
		PC->bShowMouseCursor = false;

		bIsOpenInventory = false;
	}





	//// 인벤토리를 연 경우
	//if (!InventoryWidgetInstance && !bIsOpenInventory)
	//{W
	//	if (!InventoryWidgetClass) return;

	//	// 인벤토리 위젯 인스턴스 생성 후 저장
	//	InventoryWidgetInstance = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	//	if (!InventoryWidgetInstance) return;

	//	InventoryWidgetInstance->AddToViewport();
	//	InventoryWidgetInstance->SetVisibility(ESlateVisibility::Visible);

	//	// 인벤토리 위젯 초기화
	//	InventoryWidgetInstance->InitializeInventory(InventoryComponent);

	//	// 입력 모드 설정
	//	APlayerController* PC = Cast<APlayerController>(GetController());
	//	FInputModeGameAndUI Mode;
	//	Mode.SetWidgetToFocus(InventoryWidgetInstance ? InventoryWidgetInstance->TakeWidget() : TSharedPtr<SWidget>());
	//	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//	Mode.SetHideCursorDuringCapture(false);
	//	PC->SetInputMode(Mode);

	//	// 커서 활성화
	//	PC->bShowMouseCursor = true;

	//	return;
	//}

	//// 인벤토리가 열려 있는 경우 -> 인벤토리 닫기(제거)
	//if (InventoryWidgetInstance && bIsOpenInventory)
	//{
	//	// 인벤토리 상태 플래그 활성화
	//	GetWorld()->GetTimerManager().SetTimer(InventoryTimerHandle,
	//		FTimerDelegate::CreateWeakLambda(this, [this]()
	//			{
	//				// 인벤토리 오픈 플래그 비활성화
	//				bIsOpenInventory = false;
	//			}),
	//		2.f, false
	//	);

	//	InventoryWidgetInstance->RemoveFromParent();
	//	InventoryWidgetInstance = nullptr;

	//	APlayerController* PC = Cast<APlayerController>(GetController());

	//	// 입력 모드 게임 모드로 복귀
	//	FInputModeGameOnly Mode;
	//	PC->SetInputMode(Mode);
	//	PC->bShowMouseCursor = false;
	//}
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

		// null 체크
		if (InputIds.IsEmpty())
		{
			return;
		}

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AARCharacterPlayer::GASInputPressed, InputIds[AttackAction]);

		EnhancedInputComponent->BindAction(SkillWhirlwindAction, ETriggerEvent::Started, this, &AARCharacterPlayer::GASInputHoldStart, InputIds[SkillWhirlwindAction]);
		//EnhancedInputComponent->BindAction(SkillWhirlwindAction, ETriggerEvent::Canceled, this, &AARCharacterPlayer::GASInputReleased, InputIds[SkillWhirlwindAction]);
		EnhancedInputComponent->BindAction(SkillWhirlwindAction, ETriggerEvent::Completed, this, &AARCharacterPlayer::GASInputReleased, InputIds[SkillWhirlwindAction]);

		EnhancedInputComponent->BindAction(SkillChargeAttackAction, ETriggerEvent::Started, this, &AARCharacterPlayer::GASInputHoldStart, InputIds[SkillChargeAttackAction]);
		//EnhancedInputComponent->BindAction(SkillChargeAttackAction, ETriggerEvent::Canceled, this, &AARCharacterPlayer::GASInputReleased, InputIds[SkillChargeAttackAction]);
		EnhancedInputComponent->BindAction(SkillChargeAttackAction, ETriggerEvent::Completed, this, &AARCharacterPlayer::GASInputReleased, InputIds[SkillChargeAttackAction]);
	}
}

void AARCharacterPlayer::GASInputHoldStart(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec && !Spec->IsActive())
	{
		Spec->InputPressed = true;
		ASC->TryActivateAbility(Spec->Handle);
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

void AARCharacterPlayer::ResetInventoryToggleCooldown()
{
	// 인벤토리 토글 플래그 활성화
	bCanToggleInventory = true;
}

void AARCharacterPlayer::EquipWeapon(class AARWeaponBase* EWeapon, FName SocketName)
{
	if (EWeapon && GetMesh())
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Weapon.None"))))
		{
			ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.Weapon.None")));
		}
		WeaponTag = EWeapon->WeaponTag;
		EWeapon->AttachToSocket(this, SocketName);
		ASC->AddLooseGameplayTag(EWeapon->WeaponTag);
	}
}

void AARCharacterPlayer::UnequipWeapon(AARWeaponBase* EWeapon)
{
	if (EWeapon)
	{
		ASC->RemoveLooseGameplayTag(EWeapon->WeaponTag);
		EWeapon->DetachFromCharacter();
		EWeapon->Destroy();

		WeaponTag = FGameplayTag::RequestGameplayTag("Character.Weapon.None");
		
		ASC->AddLooseGameplayTag(WeaponTag);
		CurrentWeapon = nullptr;
		
	}
}

void AARCharacterPlayer::UnequipWeaponTest(AARWeaponBase* EWeapon)
{
	if (EWeapon)
	{
		ASC->RemoveLooseGameplayTag(EWeapon->WeaponTag);
		EWeapon->DetachFromCharacter();
		EWeapon->Destroy();

		WeaponTag = FGameplayTag::RequestGameplayTag("Character.Weapon.None");

		ASC->AddLooseGameplayTag(WeaponTag);
		CurrentWeapon = nullptr;

	}
}

void AARCharacterPlayer::EquipStartWeapon()
{
	if (StartWeaponClass)
	{
		CurrentWeapon = GetWorld()->SpawnActor<AARWeaponBase>(StartWeaponClass);

		if (CurrentWeapon)
		{
			EquipWeapon(CurrentWeapon, CurrentWeapon->Socket);
		}
	}
}

void AARCharacterPlayer::PlayAction(FGameplayTag ActionTag)
{
	if (UARCharacterAnimInstance* Anim = Cast<UARCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		Anim->PlayActionMontage(WeaponTag,ActionTag);
	}
}

void AARCharacterPlayer::OnHitByAttack_Implementation(const FHitResult& HitResult, AActor* InInstigator)
{
	FVector ToInstigator = (InInstigator->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	FVector TargetForward = GetActorForwardVector();

	float Dot = FVector::DotProduct(TargetForward, ToInstigator);
	float CrossZ = FVector::CrossProduct(TargetForward, ToInstigator).Z;

	UE_LOG(LogTemp, Log, TEXT("%f"), Dot);
	FGameplayTag HitCueTag;

	if (Dot > 0.7f) HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Front");
	else if (Dot < -0.7f) HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Back");
	else if (CrossZ > 0) HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Left");
	else HitCueTag = FGameplayTag::RequestGameplayTag("GameplayCue.Hit.Right");

	FGameplayCueParameters Params;
	Params.Instigator = InInstigator;
	Params.EffectCauser = InInstigator;
	Params.Location = HitResult.ImpactPoint;
	Params.SourceObject = this;
	Params.Normal = HitResult.ImpactNormal;

	Params.AggregatedTargetTags.AddTag(HitCueTag);
	if (Params.AggregatedTargetTags.HasTag(HitCueTag))
	{
		UE_LOG(LogTemp, Log, TEXT("HitCueTag is in AggregatedTargetTags!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT(" HitCueTag is NOT in AggregatedTargetTags!"));
	}

	ASC->ExecuteGameplayCue(AruaGamePlayTags::GameplayCue_Character_AttackHit, Params);
}
