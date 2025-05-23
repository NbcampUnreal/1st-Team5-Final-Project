// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "EMaskType.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "DoggebiCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PAYROCK_API ADoggebiCharacter : public AEnemyCharacter
{
	GENERATED_BODY()

	
public:
	ADoggebiCharacter();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pattern")
	EMaskType CurrentMask;

	void AttachWeaponChangeSocket(FName NewSocketName);

	//이거는 GA애서 playmontage해서 변경해주는거로 하자.
	void ChangeMask(EMaskType NewMask);


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

	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaskMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> Mask;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	FName MaskSocketName = FName("MaskSocket");
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Mesh")
	FName BeforeCombatWeaponSocket = FName("BeforeCombatWeaponSocket");
	
};
