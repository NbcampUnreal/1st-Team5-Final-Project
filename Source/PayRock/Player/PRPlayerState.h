// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "PRPlayerState.generated.h"

class UAttributeSet;

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE(FOnExtractionDelegate);

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

public:
	FOnDeathDelegate OnDeathDelegate;
	FOnExtractionDelegate OnExtractionDelegate;

private:
	UFUNCTION()
	void OnRep_bIsDead(bool Old_bIsDead);
	UFUNCTION()
	void OnRep_bIsExtracted();

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
	UPROPERTY()
	FLinearColor ClothesColor = FLinearColor::White;
};
