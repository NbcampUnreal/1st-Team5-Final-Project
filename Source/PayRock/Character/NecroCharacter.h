// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PayRock/Input/PRInputConfig.h"
#include "NecroCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class PAYROCK_API ANecroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ANecroCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void AddCharacterAbilities() override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
protected:
	// SpringArm Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float BaseWalkSpeed = 350.f;

private:
	/*
	 *	Ability Callback for Input Tag
	 */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPRInputConfig> InputConfig;
};
