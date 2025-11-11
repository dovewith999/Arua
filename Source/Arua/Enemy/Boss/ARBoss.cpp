// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/ARBoss.h"
#include "AI/ARAIController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/MonsterAttributeSet.h"
#include "UI/Model/BossData.h"
#include "UI/ViewModel/BossViewModel.h"

AARBoss::AARBoss()
{
	// 메시 컴포넌트 설정.
	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f)
	);

	// 애셋 지정.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMesh(
		TEXT("/Game/Elemental_Dragon/Meshes/SK_Elemental_Dragon_Toon.SK_Elemental_Dragon_Toon"));

	if (CharacterMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(CharacterMesh.Object);
	}


	// 브레스 몽타주 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAttackFireBreathSwipeRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_Attack_FireBreath_Swipe.AM_Attack_FireBreath_Swipe"));
	if (MontageAttackFireBreathSwipeRef.Succeeded())
	{
		MontageAttackFireBreathSwipe = MontageAttackFireBreathSwipeRef.Object;
	}

	//왼손 공격 몽타주 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAttackPawLeftRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_Attack_Paw_Left.AM_Attack_Paw_Left"));
	if (MontageAttackPawLeftRef.Succeeded())
	{
		MontageAttackPawLeft = MontageAttackPawLeftRef.Object;
	}

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UMonsterAttributeSet>(TEXT("AttributeSet"));

	//AttributeSet = NewObject<UMonsterAttributeSet>(ASC, UMonsterAttributeSet::StaticClass());
	ASC->AddAttributeSetSubobject(AttributeSet.Get());

	AIControllerClass = AARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	//if (AttributeSet)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AttributeSet created in constructor: %s"), *AttributeSet->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("AttributeSet is NULL in constructor"));
	//}
}

//GAS

void AARBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

void AARBoss::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ASC->InitAbilityActorInfo(this, this);

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}

	VM = NewObject<UBossViewModel>();

	UBossData* Model = NewObject<UBossData>();

	if (AttributeSet == nullptr)
	{
		AttributeSet = Cast<UARAttributeSetBase>(
			const_cast<UAttributeSet*>(ASC->GetAttributeSet(UMonsterAttributeSet::StaticClass()))
		);

		UE_LOG(LogTemp, Error, TEXT("AttributeSet is NULL in constructor"));

	}

	Model->BindToAttributeSet(Cast<UMonsterAttributeSet>(AttributeSet));
	Model->SetName(TEXT("드래곤"));

	VM->Initialize(Model);
}

void AARBoss::AttackFireBreathSwipe()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackFireBreathSwipe);

	}
}

void AARBoss::AttackPawLeft()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackPawLeft);

	}
}