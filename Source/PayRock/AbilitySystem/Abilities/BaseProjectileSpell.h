// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageGameplayAbility.h"
#include "BaseProjectileSpell.generated.h"

UCLASS()
class PAYROCK_API UBaseProjectileSpell : public UBaseDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FGameplayTag& SocketTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABaseProjectile> ProjectileClass;
};
