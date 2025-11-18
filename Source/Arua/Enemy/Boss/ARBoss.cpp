// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss/ARBoss.h"
#include "AI/ARAIController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet/MonsterAttributeSet.h"
#include "UI/Model/BossData.h"
#include "UI/ViewModel/BossViewModel.h"
#include "AI/ARAI.h"
#include "Behaviortree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"


AARBoss::AARBoss()
{
	PrimaryActorTick.bCanEverTick = true;


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

	//콤보 공격 1 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageComboAttackPawLeft_TailRightRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_Attack_Combo_PawLeft_TailRight.AM_Attack_Combo_PawLeft_TailRight"));
	if (MontageComboAttackPawLeft_TailRightRef.Succeeded())
	{
		MontageComboAttackPawLeft_TailRight = MontageComboAttackPawLeft_TailRightRef.Object;
	}

	//토네이도 공격 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAttackShootTornadoRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_MakeTornado.AM_MakeTornado"));
	if (MontageAttackShootTornadoRef.Succeeded())
	{
		MontageAttackShootTornado = MontageAttackShootTornadoRef.Object;
	}

	//찍기 공격 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAttackStumpRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_Stump.AM_Stump"));
	if (MontageAttackStumpRef.Succeeded())
	{
		MontageAttackStump = MontageAttackStumpRef.Object;
	}

	//소환하는 폭발 효과 에셋 지정
	static ConstructorHelpers::FClassFinder<AActor> SpawnExplodeRef(TEXT("/Game/Blueprints/Enemy/Boss/BP_Explode.BP_Explode_C"));
	if (SpawnExplodeRef.Succeeded())
	{
		SpawnExplode = SpawnExplodeRef.Class;
	}


	//왼쪽 회전 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageTurnLeftRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_Turn_Left.AM_Turn_Left"));
	if (MontageTurnLeftRef.Succeeded())
	{
		MontageTurnLeft = MontageTurnLeftRef.Object;
	}

	//오른쪽 회전 에셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageTurnRightRef(TEXT("/Game/Animation/Enemy/Boss/Elemental_Dragon/AM_Turn_Right.AM_Turn_Right"));
	if (MontageTurnRightRef.Succeeded())
	{
		MontageTurnRight = MontageTurnRightRef.Object;
	}

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UMonsterAttributeSet>(TEXT("AttributeSet"));

	//AttributeSet = NewObject<UMonsterAttributeSet>(ASC, UMonsterAttributeSet::StaticClass());
	ASC->AddAttributeSetSubobject(AttributeSet.Get());

	AIControllerClass = AARAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

//GAS

void AARBoss::BeginPlay()
{
	Super::BeginPlay();

	//공격 범위 설정
	AARAIController* AICon = Cast<AARAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsFloat(BBKEY_ATTACKRADIUS, GetBossAttackRange());
		}
	}

	NestLocation = GetActorLocation();

	UBlackboardComponent* BB = AICon->GetBlackboardComponent();
	if (BB)
	{
		BB->SetValueAsVector (BBKEY_NESTPOS,NestLocation);
	}
	
	
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
	Model = NewObject<UBossData>();

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

////디버그용
void AARBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		DrawDebugSphere(GetWorld(), GetActorLocation(), BossAttackRange, 32, FColor::Red, false, -1.f, 0, 2.f);

}




void AARBoss::AttackFireBreathSwipe()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackFireBreathSwipe);
	}

	AARAIController* AICon = Cast<AARAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsFloat(BBKEY_WAITTIME, AttackFireBreathSwipeTime);
		}
	}


}

void AARBoss::AttackPawLeft()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{

		AnimInstance->Montage_Play(MontageAttackPawLeft);

	}

	AARAIController* AICon = Cast<AARAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsFloat(BBKEY_WAITTIME, AttackPawLeftTime);
		}
	}

}

void AARBoss::ComboAttackPawLeft_TailRight()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageComboAttackPawLeft_TailRight);

	}

	AARAIController* AICon = Cast<AARAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsFloat(BBKEY_WAITTIME, ComboAttackPawLeft_TailRightTime);
		}
	}

}

void AARBoss::AttackShootTornado()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackShootTornado);

		SpawnTornado();

	}

	AARAIController* AICon = Cast<AARAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsFloat(BBKEY_WAITTIME, AttackShootTornadoTime);
		}
	}

}

void AARBoss::AttackStump()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageAttackStump);

	}

	AARAIController* AICon = Cast<AARAIController>(GetController());
	if (AICon)
	{
		UBlackboardComponent* BB = AICon->GetBlackboardComponent();
		if (BB)
		{
			BB->SetValueAsFloat(BBKEY_WAITTIME, AttackStumpTime);
		}
		//SpawnActor(SpawnExplode, 500.0f); 
		//animnotify 에서 call 해서 eventgrpah에서 사용중이여서 굳이 호출할 필요가 없음.
	}

}

void AARBoss::TurnLeft()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageTurnLeft);
	}
}

void AARBoss::TurnRight()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(MontageTurnRight);
	}
}

void AARBoss::SpawnBossActor(TSubclassOf<AActor> ActorToSpawn, float Distance, float SetZAxis)
{
	if (!ActorToSpawn) return;

	FVector BossLocation = GetActorLocation();

	FVector Forward = GetActorForwardVector();

	FVector SpawnLocation = BossLocation + Forward * Distance;

	SpawnLocation.Z += SetZAxis;

	FRotator SpawnRotation = GetActorRotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLocation, SpawnRotation, Params);


}

void AARBoss::IsSensed(bool InIsSensed)
{
	Super::IsSensed(InIsSensed);

	Model->SetIsSensed(InIsSensed);
}
