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
	void OnBlindTagChange(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnKnockbackTagChange(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnFrozenTagChange(const FGameplayTag Tag, int32 TagCount);
	UFUNCTION()
	void OnShockedTagChange(const FGameplayTag Tag, int32 TagCount);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Client, Unreliable)
	void Client_BroadcastTagChange(const FGameplayTag& Tag, int32 TagCount);
	
	void DisableMovement();
	void EnableMovement();
	void CancelActiveAbilities();
	
	UPROPERTY()
	APRCharacter* OwningPRCharacter;

	float KnockbackForce;
	float KnockbackVertical;

	FTimerHandle MovementTimer;
};
