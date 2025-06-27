// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "PayRock/Enemy/EBossPhase.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "MukCheonWangCharacter.generated.h"

class UBaseDamageGameplayAbility;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;


UCLASS()
class PAYROCK_API AMukCheonWangCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AMukCheonWangCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	FORCEINLINE UAnimMontage* GetRoarMontage() const { return RoarMontage; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Phase")
	EBossPhase CurrentPhase;

	TArray<TWeakObjectPtr<AActor>> GetDetectedActors() {return  DetectedActors;}
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAuraEffect(UNiagaraSystem* InAuraEffect, TSubclassOf<AActor> InFontlClass, float InAuraRate);
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	
	TArray<TWeakObjectPtr<AActor>> DetectedActors;



	// 의자 및 앉는 거 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* ChairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sit")
	bool bIsSit=true;

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void ToggleVisibleChairMesh(bool isActive);

	//왕관
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* KingCrown;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	FName CrownSocketName = FName("HeadCapSocket");

	// 주먹 충돌처리
//region Boss Melee Attack
protected:
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> HitActors;

	// Sweep Params
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRadius = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackRange = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float AttackDamage = 30.f;

	FTimerHandle MeleeSweepTimerHandle;
public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeSweep(FName SocketName, UBaseDamageGameplayAbility* Ablilty);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ClearHitActors();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void StartMeleeAttack(FName SocketName, UBaseDamageGameplayAbility* Ablilty);

	void PerformMeleeSweep_Internal();
	
	FName ActiveSocketName;
	UBaseDamageGameplayAbility* CurrentAbilityRef;

	
//endregion

	
	
	
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
	void UpdateRandomTarget();
	
	void OnPhaseChanged(EBossPhase NewPhase);

	float GetHealthPercent() const;
	


private:
	UPROPERTY(EditDefaultsOnly, Category = "Boss|Animation")
	TObjectPtr<UAnimMontage> RoarMontage;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Hearing")
	float LoudnessThreshold = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "AI|Hearing")
	float MinLoudnessToReact = 50.f;
	
	
	FTimerHandle TimerHandle;



//얼굴관련

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floating Parts")
	USceneComponent* FaceCubeSceneComponent;
	
	TArray<USceneComponent*> OrbitParts; 
	TMap<USceneComponent*, FTransform> InitialTransforms;
	TMap<USceneComponent*, FVector> OrbitAxisMap; 
	TMap<USceneComponent*, float> OrbitSpeedMap; 

	UFUNCTION(BlueprintCallable, Category = "Face")
	void StartFloatingParts();
	UFUNCTION(BlueprintCallable, Category = "Face")
	void RestorePartsToInitial();
	
	bool bIsFloating = false;
	bool bRestoreToInitial = false;

private:
	void RotateFaceCube(float DeltaTime);
	
};
