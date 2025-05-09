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

	virtual void Tick(float DeltaSeconds) override;

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

	//애니메이션 상태 리플리케이션용 변수들
	//캐릭터 방향 계산
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateDirectionCustom(const FVector& Velocity, const FRotator& BaseRotation);

	// 블렌드 스페이스에 사용될 이동 방향
	UPROPERTY(ReplicatedUsing = OnRep_MoveDirection, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	float MoveDirection = 0.f;

	// 걷기/뛰기
	UPROPERTY(ReplicatedUsing = OnRep_Sprinting, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsSprinting = false;

	// 앉기
	UPROPERTY(ReplicatedUsing = OnRep_Crouching, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsCrouching = false;

	// 공중 여부 (점프/낙하)
	UPROPERTY(ReplicatedUsing = OnRep_InAir, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsInAir = false;

	// 공격 중인지
	UPROPERTY(ReplicatedUsing = OnRep_Attacking, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Combat")
	bool bIsAttacking = false;

	// 방어 중인지
	UPROPERTY(ReplicatedUsing = OnRep_Guarding, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Combat")
	bool bIsGuarding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bJustJumped = false;

	bool bResetJustJumpedNextFrame = false;
	float JustJumpedElapsedTime = 0.f;


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

	// 상호작용 관련 함수
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* FindInteractableActor() const;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 100.f;

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

	UFUNCTION()
	void OnRep_MoveDirection();

	UFUNCTION()
	void OnRep_Sprinting();

	UFUNCTION()
	void OnRep_Crouching();

	UFUNCTION()
	void OnRep_InAir();

	UFUNCTION()
	void OnRep_Attacking();

	UFUNCTION()
	void OnRep_Guarding();

	void SetJustJumped(bool bNewValue); // 인라인 가능

private:
	/* Callback functions for binding ability input actions based on Input Tags  */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPRInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
