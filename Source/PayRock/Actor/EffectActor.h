// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	NONE
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	NONE
};

USTRUCT(BlueprintType)
struct FGameplayEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectApplicationPolicy EffectApplicationPolicy = EEffectApplicationPolicy::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectLevel = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEffectRemovalPolicy EffectRemovalPolicy = EEffectRemovalPolicy::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 NumStacksToRemove = 1;
};

UCLASS()
class PAYROCK_API AEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AEffectActor();

protected:
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, const FGameplayEffectInfo& EffectInfo);
	UFUNCTION(BlueprintCallable)
	bool RemoveEffectFromTarget(AActor* TargetActor, const FActiveGameplayEffectHandle& EffectHandle, const FGameplayEffectInfo& EffectInfo);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffect")
	TArray<FGameplayEffectInfo> GameplayEffects;

	TMap<FActiveGameplayEffectHandle, FGameplayEffectInfo> ActiveEffectHandleInfoMap;
};
