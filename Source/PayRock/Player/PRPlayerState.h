// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PRPlayerState.generated.h"

class UAttributeSet;

UCLASS()
class PAYROCK_API APRPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APRPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	FORCEINLINE int32 GetCharacterLevel() const { return Level; };

	FORCEINLINE bool GetIsDead() const { return bIsDead; };
	void SetIsDead(bool bDead);

private:
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, Replicated)
	bool bIsDead = false;
};
