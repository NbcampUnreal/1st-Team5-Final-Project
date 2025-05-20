// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "MarketClownMonster.generated.h"

UENUM(BlueprintType)
enum class ETalMaskType : uint8
{
	Yangban UMETA(DisplayName = "양반"),
	Imae UMETA(DisplayName = "이매"),
	Baekjeong UMETA(DisplayName = "백정"),
	Bune UMETA(DisplayName = "부네")
};

UCLASS()
class PAYROCK_API AMarketClownMonster : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AMarketClownMonster();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ApplyMaskBehavior(ETalMaskType MaskType);
	UFUNCTION(BlueprintCallable)
	void SplitOnDeath();
	UFUNCTION(BlueprintCallable)
	void ApplySplitLevelAttributes(int32 InLevel);
	
	UAnimMontage* GetCurrentMaskAttackMontage() const;
	UAnimMontage* GetRoarMontage() const;
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* GetBlackboardTarget() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	ETalMaskType CurrentMask;

	

protected:

	virtual void Die() override;
	

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	TObjectPtr<USphereComponent> WeaponCollision;

	UPROPERTY(VisibleAnywhere, Category = "Mask")
	USkeletalMeshComponent* MaskMesh;

	UPROPERTY(EditAnywhere, Category = "Mask")
	USkeletalMesh* MaskMesh_Yangban;

	UPROPERTY(EditAnywhere, Category = "Mask")
	USkeletalMesh* MaskMesh_Imae;

	UPROPERTY(EditAnywhere, Category = "Mask")
	USkeletalMesh* MaskMesh_Baekjeong;

	UPROPERTY(EditAnywhere, Category = "Mask")
	USkeletalMesh* MaskMesh_Bune;

	UPROPERTY(EditAnywhere, Category = "Mask")
	UAnimMontage* RoarMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName HeadSocketName = FName("HeadSocket");
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName CollisionSocketName = FName("CollisionSocket");
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> StrengthOverrideEffect;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float YangbanSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float ImaeSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float BaekjeSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float BuneSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (AllowPrivateAccess = "true"))
	int32 SplitLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (AllowPrivateAccess = "true"))
	int32 MaxSplitCount = 2;
};
