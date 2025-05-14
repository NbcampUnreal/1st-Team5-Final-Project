// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageGameplayAbility.h"
#include "BaseProjectileSpell.generated.h"

UCLASS()
class PAYROCK_API UBaseProjectileSpell : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FGameplayTag& SocketTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABaseProjectile> ProjectileClass;

	bool bSpawned;
};
