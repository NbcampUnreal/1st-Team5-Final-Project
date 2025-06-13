// PayRockGames

#pragma once

#include "CoreMinimal.h"
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

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
	void Client_ApplyDebuffBlind();
	UFUNCTION(Client, Reliable)
	void Client_RemoveDebuffBlind();

private:
	UPROPERTY()
	APRCharacter* OwningPRCharacter;
};
