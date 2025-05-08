// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PRCharacter.generated.h"

class UPRInputConfig;
class USpringArmComponent; // 스프링 암 관련 클래스 헤더
class UCameraComponent; // 카메라 관련 클래스 전방 선언
struct FInputActionValue; // Enhanced Input에서 액션 값을 받을 때 사용하는 구조체

UCLASS()
class PAYROCK_API APRCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APRCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// SpringArm Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// Move Speed Property
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float BackwardSpeedMultiplier;

	// Mouse Sensitivity
	UPROPERTY(EditAnywhere, Category = "Input")
	float MouseSensitivity;

	/** Combat Interface */
	virtual int32 GetCharacterLevel() override;
	/** end Combat Interface */


protected:
	virtual void AddCharacterAbilities() override;
	
private:
	virtual void InitAbilityActorInfo() override;
	void SetupStimuliSource();
	void SetSpeed(float NewSpeedMultiplier);

protected:
	//AI 감지 관련 함수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IA_Move와 IA_Jump 등을 처리할 함수 원형
	// Enhanced Input에서 액션 값은 FInputActionValue로 전달
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StopCrouch(const FInputActionValue& value);
	UFUNCTION()
	void StartAttack(const FInputActionValue& value);
	UFUNCTION()
	void StopAttack(const FInputActionValue& value);
	UFUNCTION()
	void StartGuard(const FInputActionValue& value);
	UFUNCTION()
	void StopGuard(const FInputActionValue& value);
	UFUNCTION()
	void Interact(const FInputActionValue& value);

private:
	/* Callback functions for binding ability input actions based on Input Tags  */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPRInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
