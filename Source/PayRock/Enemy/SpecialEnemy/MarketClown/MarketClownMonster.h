// PayRockGames

#pragma once

#include "CoreMinimal.h"
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
	void InitSplitLevel(int32 InLevel);
	
	UAnimMontage* GetCurrentMaskAttackMontage() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	ETalMaskType CurrentMask;

	

protected:

	virtual void Die() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "MaskAttack")
	TArray<TObjectPtr<UAnimMontage>> MaskAttackMontages;

	UPROPERTY(VisibleAnywhere, Category = "Mask")
	USkeletalMeshComponent* MaskMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Mask")
	USkeletalMesh* MaskMesh_Yangban;

	UPROPERTY(EditDefaultsOnly, Category = "Mask")
	USkeletalMesh* MaskMesh_Imae;

	UPROPERTY(EditDefaultsOnly, Category = "Mask")
	USkeletalMesh* MaskMesh_Baekjeong;

	UPROPERTY(EditDefaultsOnly, Category = "Mask")
	USkeletalMesh* MaskMesh_Bune;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float YangbanSpeed = 400.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float ImaeSpeed = 650.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float BaekjeSpeed = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaskMovement", meta = (AllowPrivateAccess = "true"))
	float BuneSpeed = 450.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Split", meta = (AllowPrivateAccess = "true"))
	int32 SplitLevel = 0;
};
