// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/AbilitySystem/Abilities/BaseDamageGameplayAbility.h"
#include "PayRock/Enemy/MiddleBoss/Doggebi/DoggebiCharacter.h"
#include "GA_Doggebi_YellowPatternAttack.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API UGA_Doggebi_YellowPatternAttack : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Doggebi_YellowPatternAttack();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	
protected:
	UFUNCTION(BlueprintCallable)
	void BindCallbackToWeaponCollision();

	UFUNCTION()
	void OnWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void ToggleColiision(bool isActive);
	
	UFUNCTION(BlueprintCallable)
	void ResetDamaegdActors();

private:
	UFUNCTION(BlueprintCallable)
	void EndAttackBlackboardState();

	UPROPERTY()
	ADoggebiCharacter* Doggebi;

	UPROPERTY()
	TSet<AActor*> DamagedActors;

};
