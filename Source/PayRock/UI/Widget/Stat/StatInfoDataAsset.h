// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatInfoDataAsset.generated.h"

/**
 * ���� �ϳ��� ���� ����ü (ǥ���̸� + ����)
 */

USTRUCT(BlueprintType)
struct FStatInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FString StatName; // ���� Ű (��: "Strength")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FText StatDisplayName; // ǥ�� �̸� (��: "��")

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	FText Description; // ���� ���� (��: "���� ���� ���ݷ��� ������ŵ�ϴ�.")
};

/**
 * ��ü ���� ������ ��� ������ ����
 */


UCLASS()
class PAYROCK_API UStatInfoDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TArray<FStatInfo> StatList;

	// ���� �߰��Ǵ� �� (��Ÿ�ӿ�, ������ ���� X)
	TMap<FName, FStatInfo> StatInfoMap;


public:
	// ��Ÿ�� �� �ʱ�ȭ
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
