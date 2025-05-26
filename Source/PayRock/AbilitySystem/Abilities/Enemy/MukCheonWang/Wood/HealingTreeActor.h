// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PayRock/Enemy/FinalBoss/MukCheonWangCharacter.h"
#include "GameplayEffectExtension.h"
#include "HealingTreeActor.generated.h"

UCLASS()
class PAYROCK_API AHealingTreeActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHealingTreeActor();
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void SetBoss(AMukCheonWangCharacter* InBoss);
	void InitAbilityActorInfo();
	void InitializeDefaultAttributes() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	const UPRAttributeSet* AttributeSet = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Healing")
	float HealPerSecond = 1.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Healing")
	float TickInterval = 1.0f;

	UPROPERTY()
	TObjectPtr<AMukCheonWangCharacter> Boss;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitPrimaryAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitSecondaryAttributeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> InitVitalAttributeEffect;

	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void HealBoss();
};