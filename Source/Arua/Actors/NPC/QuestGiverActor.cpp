// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/NPC/QuestGiverActor.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Blueprint/UserWidget.h"
#include "UI/Dialog/DialogWidget.h"
#include "UI/Quest/QuestSelectWidget.h"

AQuestGiverActor::AQuestGiverActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 상호작용 콜리전 박스 CDO 생성
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetBoxExtent(FVector(100, 100, 100));
	RootComponent = InteractionVolume;

	// UI 카메라 컴포넌트 CDO 생성
	UICameraActor = CreateDefaultSubobject<UCameraComponent>(TEXT("UICameraActor"));
	UICameraActor->SetupAttachment(RootComponent);

	// NPC 위젯 컴포넌트 COD 생성
	NPCWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCWidgetComponent"));
	NPCWidgetComponent->SetupAttachment(RootComponent);
}

void AQuestGiverActor::Interact(APawn* InInteractor)
{
	// 플레이어 컨트롤러 가져오기 및 저장
	PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC || !InInteractor || bInteracting || !UICameraActor)
	{
		return;
	}

	// 상호작용 플레이어 초기화
	InteractorPawn = InInteractor;

	// #1: 플레이어 카메라 -> UI 카메라로 전환
	OriginalViewTarget = PC->GetViewTarget();
	PC->SetViewTargetWithBlend(this, ViewTargetBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	// #3: Dialog 위젯 생성 및 표시
	if (DialogWidgetClass && !DialogWidgetInstance)
	{
		DialogWidgetInstance = CreateWidget<UDialogWidget>(PC, DialogWidgetClass);
		if (DialogWidgetInstance)
		{
			// 다이얼로그 위젯 그리기
			DialogWidgetInstance->AddToViewport();

			// 다이얼로그 키 입력 이벤트와 상호작용 종료 함수 바인딩
			DialogWidgetInstance->OnRequestInput.AddDynamic(this, &AQuestGiverActor::UnInteract);
		}
	}

	// #4: 퀘스트 선택 위젯 생성 및 표시
	if (QuestSelectWidgetClass && !QuestSelectWidgetInstance)
	{
		QuestSelectWidgetInstance = CreateWidget<UQuestSelectWidget>(PC, QuestSelectWidgetClass);
		if (QuestSelectWidgetInstance)
		{
			// 퀘스트 리스트 초기화
			QuestSelectWidgetInstance->InitQuestList(ProvidedQuestIDs, QuestDataTables);
			
			// 퀘스트 선택 위젯 그리기
			QuestSelectWidgetInstance->AddToViewport();
		}
	}

	// #5: UI 상호작용 모드 설정 (입력 등)
	ApplyUIInteractionMode();

	// #6: 상호작용 상태 플래그 활성화
	GetWorld()->GetTimerManager().SetTimer(ViewTargetBlendTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				bInteracting = true;
			}),
		ViewTargetBlendTime,
		false
	);
}

void AQuestGiverActor::UnInteract()
{
	if (!PC || !bInteracting || !UICameraActor) return;

	// #1: UI 카메라 -> 플레이어 카메라로 전환
	PC->SetViewTargetWithBlend(OriginalViewTarget, ViewTargetBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	// #2: Dialog 위젯 정리
	if (DialogWidgetInstance)
	{
		DialogWidgetInstance->RemoveFromParent();
		DialogWidgetInstance = nullptr;
	}

	// #3: 퀘스트 선택 위젯 정리
	if (QuestSelectWidgetInstance)
	{
		QuestSelectWidgetInstance->RemoveFromParent();
		QuestSelectWidgetInstance = nullptr;
	}

	// #4: 게임 모드 설정 (입력 등)
	RestoreGameplayMode();

	// #5: 상태 초기화
	GetWorld()->GetTimerManager().SetTimer(ViewTargetBlendTimer,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				bInteracting = false;
				InteractorPawn = nullptr;
				PC = nullptr;
				OriginalViewTarget = nullptr;
			}),
		ViewTargetBlendTime,
		false
	);
}

void AQuestGiverActor::BeginPlay()
{
	Super::BeginPlay();
}

TArray<FQuestData> AQuestGiverActor::GetProvidedQuests() const
{
	// 이 NPC가 제공할 QuestID 리스트
	TArray<FQuestData> ProvidedQuests;

	// 퀘스트 데이터 테이블이 없으면 반환
	if (!QuestDataTables)
	{
		return ProvidedQuests;
	}

	// ProvidedQuestIDs가 비어있는 경우
	if (ProvidedQuestIDs.Num() == 0)
	{
		static const FString ContextString(TEXT("QuestGiver_GetAll"));
		TArray<FName> RowNames = QuestDataTables->GetRowNames();

		// DataTable의 모든 행을 QuestID 리스트에 추가
		for (const FName& QuestID : RowNames)
		{
			FQuestData* Data = QuestDataTables->FindRow<FQuestData>(QuestID, ContextString);
			if (Data)
			{
				ProvidedQuests.Add(*Data);
			}
		}
	}
	else
	{
		static const FString ContextString(TEXT("QuestGiver_GetProvided"));

		// 제공할 QuestID 리스트만 QuestID 리스트에 추가
		for (const FName& QuestID : ProvidedQuestIDs)
		{
			FQuestData* Data = QuestDataTables->FindRow<FQuestData>(QuestID, ContextString);
			if (Data)
			{
				ProvidedQuests.Add(*Data);
			}
		}
	}

	return ProvidedQuests;
}

void AQuestGiverActor::ApplyUIInteractionMode()
{
	if (!InteractorPawn || !PC) return;

	// 상호작용 플레이어 메시 비활성화
	USkeletalMeshComponent* Mesh = InteractorPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (Mesh)
	{
		Mesh->SetVisibility(false, true);
	}

	// 커서 활성화 및 컨트롤러 입력 비활성화
	PC->bShowMouseCursor = true;
	PC->SetIgnoreLookInput(true);
	PC->SetIgnoreMoveInput(true);
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;

	// 입력 모드 UI Only로 설정 및 다이얼로그 위젯으로 포커싱
	FInputModeGameAndUI Mode;
	Mode.SetWidgetToFocus(DialogWidgetInstance ? DialogWidgetInstance->TakeWidget() : TSharedPtr<SWidget>());
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Mode.SetHideCursorDuringCapture(false);
	PC->SetInputMode(Mode);
}

void AQuestGiverActor::RestoreGameplayMode()
{
	if (!InteractorPawn || !PC) return;

	// 상호작용 플레이어 메시 활성화
	USkeletalMeshComponent* Mesh = InteractorPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (Mesh)
	{
		Mesh->SetVisibility(true, true);
	}

	// 커서 비활성화 및 컨트롤러 입력 활성화
	PC->bShowMouseCursor = false;
	PC->SetIgnoreLookInput(false);
	PC->SetIgnoreMoveInput(false);
	PC->bEnableClickEvents = false;
	PC->bEnableMouseOverEvents = false;

	// 입력 모드 게임 모드로 복귀
	FInputModeGameOnly Mode;
	PC->SetInputMode(Mode);
}
