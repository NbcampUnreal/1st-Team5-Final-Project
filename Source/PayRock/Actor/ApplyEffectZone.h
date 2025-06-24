// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "ApplyEffectZone.generated.h"

UCLASS(Blueprintable)
class PAYROCK_API AApplyEffectZone : public AActor
{
	GENERATED_BODY()

public:
	AApplyEffectZone();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaySoundAtLocation(USoundBase* Sound);
	void PlaySoundIfValid();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void RemoveEffectFromActor(AActor* TargetActor);

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle EffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	USoundBase* ActivationSound;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "Ability")
    bool bAttachToInstigator;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float DistanceFromInstigator;

	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bShouldApplyToInstigator;

	UPROPERTY(EditAnywhere, Category = "Ability")
	bool bOnlyApplyToPlayerCharacters;

	UPROPERTY()
	TMap<AActor*, FActiveGameplayEffectHandle> ActiveEffectMap;
};
