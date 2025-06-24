// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NavigationInvokerComponent.h"
#include "Components/SphereComponent.h"
#include "PayRock/Character/BaseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PAYROCK_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void AddCharacterAbilities() override;
	
	void ToggleWeaponCollision(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomAttackMontage() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomDetectMontage() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetBattleState() const { return bIsBattle; }

	void SetBattleState(bool Value) { bIsBattle = Value; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (AllowPrivateAccess = "true"))
	int32 SplitLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split")
	bool bIsClone = false;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayGradualSound(USoundBase* InSound, float MaxDistance, float Volume, float Loudness);
	void PlayGradualSound(USoundBase* InSound, float MaxDistance, float Volume, float Loudness);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDetectMontage(UAnimMontage* Montage);
	void PlayDetectMontage(UAnimMontage* Montage);
	
	//Sound
	UFUNCTION(BlueprintCallable,NetMulticast, Unreliable)
	void Multicast_PlayAttackSound(USoundBase* Sound);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	USoundAttenuation* AttackAttenuation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="LOD")
	UStaticMesh* LODStaticMesh;

private:
	FTimerHandle ServerDistanceCheckHandle;
	void Server_CheckPlayerProximity();
	bool bIsServerFar = false;
	
	USphereComponent* ActivationSphere;
	float ActivationRadius;
	
	TSet<APawn*> OverlappingPlayers;
	bool bIsActivated;

	UFUNCTION()
	void OnActivationOverlapBegin(UPrimitiveComponent* Overlapped, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Sweep);

	UFUNCTION()
	void OnActivationOverlapEnd(UPrimitiveComponent* Overlapped, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void SetActivated(bool bNewActive);
	bool IsPlayerCurrentlyDetected() const;
protected:
	virtual void Die(FVector HitDirection = FVector::ZeroVector) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsBattle = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	bool bIsDead = false;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<AActor> ContainerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> DetectMontages;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UPawnNoiseEmitterComponent> PawnNoiseEmitterComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation")
	UNavigationInvokerComponent* NavInvoker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LOD", meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* LODMeshComp;
};
