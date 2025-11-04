#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestData.generated.h"

// 퀘스트 타입 구조체 (퀘스트 종류)
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	Kill,       // 처치 퀘스트 (ex. 드래곤 3마리 처치)
	Collect,    // 수집 퀘스트 (ex. 허브 5개 수집)
	Talk,       // 대화 퀘스트 (ex. 임무 게시판과 대화하기)
	Custom
};

USTRUCT(BlueprintType)
struct FQuestData : public FTableRowBase
{
	GENERATED_BODY()

	// 퀘스트 ID: 퀘스트의 고유 키
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	// 퀘스트 타이틀: 퀘스트 제목/이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	// 퀘스트 설명: 퀘스트에 대한 상세 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
	FText Description;

	// 퀘스트 타입: 퀘스트 종류
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType Type;

	// 퀘스트 목표 수치: 퀘스트 완료 조건 목표 수치 (ex. Kill 5, Collect 3 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ObjectiveCount = 0;

	// 퀘스트 타깃 ID: 퀘스트의 대상 오브젝트의 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectiveTarget; // 예: 몬스터 ID, 아이템 ID

	// 퀘스트 완료 보상: 퀘스트 완료 시 지급되는 골드량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RewardGold = 0;
};