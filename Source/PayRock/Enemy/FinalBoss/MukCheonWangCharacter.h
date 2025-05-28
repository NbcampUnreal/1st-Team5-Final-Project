// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "Perception/AIPerceptionTypes.h"
#include "MukCheonWangCharacter.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1,
	Phase2,
	Phase3
};

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

	TArray<AActor*> GetDetectedActors() {return  DetectedActors;}

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;

	UPROPERTY()
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
	
	TArray<AActor*> DetectedActors;



	// 의자 및 앉는 거 추가
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* ChairMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sit")
	bool bIsSit;

	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void ToggleVisibleSkeletalMesh(bool isActive);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* KingCrown;
	
	UPROPERTY(EditAnywhere, Category = "Mesh")
	FName CrownSocketName = FName("HeadCapSocket");

	
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
};
