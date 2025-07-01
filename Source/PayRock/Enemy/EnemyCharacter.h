// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NavigationInvokerComponent.h"
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
	virtual void BindToTagChange() override;

	// === Combat ===
	void ToggleWeaponCollision(bool bEnable);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomAttackMontage() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	UAnimMontage* GetRandomDetectMontage() const;

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_PlayAttackSound(USoundBase* Sound);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* AttackAttenuation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponSecond;
	// === Detection ===
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayDetectMontage(UAnimMontage* Montage);

	void PlayDetectMontage(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayGradualSound(USoundBase* InSound, float MaxDistance, float Volume, float Loudness);

	void PlayGradualSound(USoundBase* InSound, float MaxDistance, float Volume, float Loudness);

	// === State ===
	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool GetBattleState() const { return bIsBattle; }

	void SetBattleState(bool Value) { bIsBattle = Value; }

	UFUNCTION()
	void DisableAnimInstance();

	UFUNCTION()
	void RestoreAnimInstance();


	// === Spawn Handling ===
	TSubclassOf<AActor> GetSpawnedActor() { return SpawnerClass; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (AllowPrivateAccess = "true"))
	int32 SplitLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split")
	bool bIsClone = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	TSubclassOf<AActor> SpawnerClass;

	// === Sleep System ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sleep")
	float SleepStartTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sleep")
	bool bIsSleeping = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sleep")
	float MaxSleepDuration = 30.f;

	// ===	Debuff Component ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DeBuff")
	class UBuffComponent* BuffComponent;

protected:
	virtual void Die(FVector HitDirection = FVector::ZeroVector) override;

	// === State ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsBattle = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsDead = false;

	// === AI / Perception ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSourceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UPawnNoiseEmitterComponent> PawnNoiseEmitterComp;

	// === Animation ===
	UPROPERTY()
	TSubclassOf<UAnimInstance> SavedAnimClass;

	// === Assets ===
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<AActor> ContainerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TArray<TObjectPtr<UAnimMontage>> DetectMontages;
};