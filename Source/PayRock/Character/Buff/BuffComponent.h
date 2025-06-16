// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BuffDataAsset.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

struct FGameplayTag;
class APRCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PAYROCK_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuffComponent();

	UFUNCTION()
	void OnDebuffBlindChanged(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnDebuffKnockbackChanged(const FGameplayTag Tag, int32 TagCount);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ReactToTagChange(const FGameplayTag& Tag, int32 TagCount);
	UFUNCTION()
	void DisableMovement();
	UFUNCTION()
	void EnableMovement();

private:
	UPROPERTY()
	APRCharacter* OwningPRCharacter;

	float KnockbackForce;
	float KnockbackVertical;

	FTimerHandle MovementTimer;
};
