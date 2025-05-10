// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayAbility.h"
#include "BaseProjectileSpell.generated.h"

UCLASS()
class PAYROCK_API UBaseProjectileSpell : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FGameplayTag& SocketTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ABaseProjectile> ProjectileClass;
};
