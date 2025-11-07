// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTables/QuestData.h"
#include "Interface/AR_NPCInteractionInterface.h"
#include "QuestGiverActor.generated.h"

// 위젯 가시화 기록 구조체
USTRUCT()
struct FWidgetVisibilityRecord
{
	GENERATED_BODY()

	TWeakObjectPtr<UUserWidget> Widget;	// 위젯 클래스
	ESlateVisibility PrevVisibility;	// 이전 가시화 여부
};

UCLASS()
class ARUA_API AQuestGiverActor : public AActor, public IAR_NPCInteractionInterface
{
	GENERATED_BODY()

public:
	AQuestGiverActor();

	// 상호작용 시작 함수 (카메라 전환)
	UFUNCTION(BlueprintCallable)
	void Interact(APawn* InInteractor);

	// 상호작용 시작 인터페이스
	virtual void PlayInteraction(APawn* InInteractor) override;

	// 상호작용 종료 함수 (카메라 전환)
	UFUNCTION(BlueprintCallable)
	void UnInteract();

	// 퀘스트 수락 콜백 함수
	UFUNCTION()
	void HandleQuestAccepted(FName QuestID);

	// 퀘스트 보상 수령 콜백 함수
	UFUNCTION()
	void HandleQuestTurnIn(FName QuestID);

protected:
	virtual void BeginPlay() override;

	// UI 상호작용 모드 설정 함수 (입력 등)
	void ApplyUIInteractionMode();

	// 게임 모드 설정 함수 (입력 등)
	void RestoreGameplayMode();

	// 이 NPC가 제공하는 퀘스트 리스트 Getter
	TArray<FQuestData> GetProvidedQuests() const;

protected:
	// 상호작용 콜리전
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> InteractionVolume;

	// UI 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> UICameraActor;

	// 카메라 전환 블랜딩 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ViewTargetBlendTime = 0.25f;

	// 다이얼로그 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest | UI")
	TSubclassOf<class UDialogWidget> DialogWidgetClass;

	// NPC 위젯 컴포넌트 (NPC 이름, 퀘스트 여부 등)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest | UI")
	TObjectPtr<class UWidgetComponent> NPCWidgetComponent;

	// 퀘스트 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<class UDataTable> QuestDataTables;

	// 이 NPC가 제공할 QuestID 리스트 (없으면 DataTable의 전체를 사용)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TArray<FName> ProvidedQuestIDs;

	// 퀘스트 선택 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
	TSubclassOf<class UQuestSelectWidget> QuestSelectWidgetClass;

private:
	// 상호작용 상태 플래그
	UPROPERTY()
	bool bInteracting = false;

	// 상호작용 플레이어
	UPROPERTY()
	APawn* InteractorPawn = nullptr;

	// 상호작용 컨트롤러
	UPROPERTY()
	APlayerController* PC = nullptr;

	// 기존 뷰 타겟
	UPROPERTY()
	AActor* OriginalViewTarget;

	// 카메라 전환 타이머 핸들
	FTimerHandle ViewTargetBlendTimer;

private:
	// 다이얼로그 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UDialogWidget> DialogWidgetInstance = nullptr;

	// 퀘스트 선택 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<class UQuestSelectWidget> QuestSelectWidgetInstance = nullptr;

	// PlayerCharacter 별로 숨긴 위젯과 이전 상태를 저장
	static TMap<TWeakObjectPtr<APlayerController>, TArray<FWidgetVisibilityRecord>> CachedWidgetStates;
};
