// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PayRock/UI/Widget/Skill/SkillData.h"
#include "PRPlayerState.generated.h"

class UAttributeSet;

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExtractionDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponSkillChangedDelegate, const FSkillData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAccessorySkillChangedDelegate, const FSkillData&);

UCLASS()
class PAYROCK_API APRPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APRPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	
	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const { return bIsDead; };
	UFUNCTION(BlueprintCallable)
	void SetIsDead(bool bDead);
	UFUNCTION(BlueprintCallable)
	bool GetIsExtracted() const { return bIsExtracted; }
	UFUNCTION(BlueprintCallable)
	void SetIsExtracted(bool bExtracted);
	UFUNCTION(BlueprintCallable)
	void ForceDeath();
	UFUNCTION(BlueprintCallable)
    void Extract();

	UFUNCTION(BlueprintCallable)
	FLinearColor GetClothesColor() const { return ClothesColor; }
	UFUNCTION(BlueprintCallable)
	void SetClothesColor(const FLinearColor& Color) { ClothesColor = Color; }

	UFUNCTION(BlueprintCallable)
	void UpdateAccessoryID(const FName& ID);
	UFUNCTION(BlueprintCallable)
	void UpdateWeaponID(const FName& ID);

public:
	FOnDeathDelegate OnDeathDelegate;
	FOnExtractionDelegate OnExtractionDelegate;
	FOnAccessorySkillChangedDelegate OnAccessorySkillChangedDelegate;
	FOnWeaponSkillChangedDelegate OnWeaponSkillChangedDelegate;

private:
	UFUNCTION()
	void OnRep_bIsDead(bool Old_bIsDead);
	UFUNCTION()
	void OnRep_bIsExtracted();
	UFUNCTION()
	void OnRep_AccessoryID();
	UFUNCTION()
	void OnRep_WeaponID();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(ReplicatedUsing = OnRep_bIsDead)
	bool bIsDead = false;
	UPROPERTY(ReplicatedUsing = OnRep_bIsExtracted)
	bool bIsExtracted = false;
	UPROPERTY(ReplicatedUsing = OnRep_AccessoryID)
	FName AccessoryID;
	UPROPERTY(ReplicatedUsing = OnRep_WeaponID)
	FName WeaponID;
	UPROPERTY()
	FLinearColor ClothesColor = FLinearColor::White;
};
