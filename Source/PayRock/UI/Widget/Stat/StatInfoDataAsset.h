// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatInfoDataAsset.generated.h"

/**
 * 스탯 하나의 정보 구조체 (표시이름 + 설명)
 */

USTRUCT(BlueprintType)
struct FStatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FString StatName; // 내부 키 (예: "Strength")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FText StatDisplayName; // 표시 이름 (예: "힘")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FText Description; // 툴팁 설명 (예: "힘은 물리 공격력을 증가시킵니다.")
};

/**
 * 전체 스탯 정보를 담는 데이터 에셋
 */


UCLASS()
class PAYROCK_API UStatInfoDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TArray<FStatInfo> StatList;

	// 새로 추가되는 맵 (런타임용, 에디터 노출 X)
	TMap<FName, FStatInfo> StatInfoMap;


public:
	// 런타임 시 초기화
	virtual void PostLoad() override
	{
		Super::PostLoad();

		StatInfoMap.Empty();
		for (const FStatInfo& Info : StatList)
		{
			StatInfoMap.Add(FName(*Info.StatName), Info);
		}
	}
};
