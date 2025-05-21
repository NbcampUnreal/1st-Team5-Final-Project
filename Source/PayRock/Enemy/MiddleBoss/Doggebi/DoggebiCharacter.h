// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "DoggebiCharacter.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMaskType : uint8
{

	Red     UMETA(DisplayName = "Red"),
	Blue    UMETA(DisplayName = "Blue"),
	Yellow  UMETA(DisplayName = "Yellow")
};


UCLASS()
class PAYROCK_API ADoggebiCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

	
public:
	ADoggebiCharacter();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern")
	EMaskType CurrentMask = EMaskType::Red;
	
	
	//이거는 GA애서 playmontage해서 변경해주는거로 하자.
	UFUNCTION(Blueprintable)
	void ChangeMask(EMaskType NewMask);

	UFUNCTION(Blueprintable)
	void RandomChangeMask();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask")
	EMaskType CurrentMaskType;

	void AttachWeaponChangeSocket(FName NewSocketName);
	
	void TryExecutePattern();

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> RedAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> BlueAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<UGameplayAbility> YellowAbility;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsChoosing = false;

	void OnPatternEnd();



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	UCapsuleComponent* WeaponCollision;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaskMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Mask;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	FName MaskSocketName = FName("MaskSocket");
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	FName BeforeCombatWeaponSocket = FName("BeforeCombatWeaponSocket");


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	UCapsuleComponent* FireCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pattern")
	TArray<ATargetPoint*> TargetActors;

	//For Test
	void CycleMask();

	FTimerHandle MaskChangeTimer;
	int32 CurrentMaskIndex = 0;
	
};
