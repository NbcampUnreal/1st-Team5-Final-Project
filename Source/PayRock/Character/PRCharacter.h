// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PayRock/Item/PRItemEnum.h"
#include "Blueprint/UserWidget.h"
#include "PayRock/AbilitySystem/PRAttributeSet.h"
#include "PRCharacter.generated.h"

class UBoxComponent;
class UNiagaraSystem;
class USphereComponent;
class UPRInputConfig;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
class UPRAttributeSet;

UCLASS()
class PAYROCK_API APRCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APRCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void Tick(float DeltaSeconds) override;
	
	bool GetbIsDead() const { return bIsDead; }
	bool GetbIsExtracted() const { return bIsExtracted; }
	bool GetbIsInvisible() const { return bIsInvisible; }

	/* Components */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	USkeletalMeshComponent* GetWeapon2() const { return Weapon2; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimuliSourceComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	USphereComponent* LeftHandCollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	USphereComponent* RightHandCollisionComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionComponent")
	UBoxComponent* WeaponCollision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Weapon2;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName Weapon2SocketName = FName("Weapon2Socket");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blessing")
	class UBlessingComponent* BlessingComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blessing")
	class UBuffComponent* BuffComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	
	/* Death */
	virtual void Die(FVector HitDirection = FVector::ZeroVector) override;
	UPROPERTY(Replicated)
	bool bIsDead = false;

	/* 공격/피격 */
	UFUNCTION(Server, Reliable)
	void ServerPlayAttackSound(USoundBase* Sound, FVector Location);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayAttackSound(USoundBase* Sound, FVector Location);
	
	UFUNCTION(Client, Reliable)
	void Client_PlayHitMarker();
	void ShakeCamera();
	void PlayHitOverlay();
	void PlayHitMarker();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HitOverlayWidgetClass;
	UPROPERTY()
	UUserWidget* HitOverlayWidget = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HitMarkerWidgetClass;
	UPROPERTY()
	UUserWidget* HitMarkerWidget = nullptr;
	
	FTimerHandle CameraShakeTimerHandle;

	/* Extraction */
	UFUNCTION()
	void OnExtraction();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastExtraction();
	UFUNCTION()
	void HideCharacter();
	
	UPROPERTY(EditDefaultsOnly, Category = "Extraction")
	UNiagaraSystem* ExtractionNiagara;
	UPROPERTY(EditDefaultsOnly, Category = "Extraction")
	float HideDelay = 2.5f;
	UPROPERTY(Replicated)
	bool bIsExtracted = false;

	/* Status */
	UPROPERTY(Replicated)
	bool bIsInvisible = false;

	/* Combo */
	UFUNCTION(BlueprintCallable, Category = "Combo")
	void StartComboTimer();
	UFUNCTION()
	void SetResetCombo();
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Combo")
	bool bResetCombo = false;
	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	float ComboTime;
	FTimerHandle ComboTimerHandle;

	/* Camera */
	/* Aim */
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bNewAiming);
	UFUNCTION()
	void OnRep_IsAiming();

	//관전용 카메라 컨트롤 Replication
	UPROPERTY(Replicated)
	FRotator ReplicatedControlRotation;
	float DefaultArmLength;
	FVector DefaultSocketOffset = FVector::ZeroVector;
	float AimingArmLength;
	FVector AimingSocketOffset = FVector(0.f, 20.f, 30.f);
	float CameraInterpSpeed;

	/*** Movement ***/
	/* 속도 Speed */
	UFUNCTION()
	void OnRep_MaxWalkSpeed();
	void SetSpeedMode(bool bSprintState);
	void SetCrouchSpeed();
	void ResetToWalkSpeed();

	UPROPERTY(ReplicatedUsing = OnRep_MaxWalkSpeed)
	float ReplicatedMaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CurrentTargetSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Interp")
	float CurrentInterpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CrouchSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float BackwardSpeedMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimingSpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Interp")
	float SpeedInterpRateSprint = 6.f;
	UPROPERTY(EditDefaultsOnly, Category = "Movement|Interp")
	float SpeedInterpRateWalk = 3.f;
	UPROPERTY(EditAnywhere, Category = "Input")
	float MouseSensitivity;

	/* 방향 Direction */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateDirectionCustom(const FVector& Velocity, const FRotator& BaseRotation);
	UPROPERTY(ReplicatedUsing = OnRep_MoveDirection, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	float MoveDirection = 0.f;

	/* 점프 Jump */
	UFUNCTION(Server, Reliable)
	void ServerStartJump();
	UFUNCTION(BlueprintCallable, Category = "Anim|Movement")
	float GetLastJumpDirection() const { return LastJumpDirection; }
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	float LastJumpDirection = 0.f;
	UPROPERTY(ReplicatedUsing = OnRep_InAir, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsInAir = false;
	UPROPERTY(ReplicatedUsing = OnRep_JustJumped, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bJustJumped = false;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsDoubleJumping = false;
	bool bResetJustJumpedNextFrame = false;
	float JustJumpedElapsedTime = 0.f;

	/* 착지 Land */
	UFUNCTION(Server, Reliable)
	void ServerRequestLandingSound(FVector Location, USoundBase* Sound);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayLandingSound(FVector Location, USoundBase* Sound);
	USoundBase* GetLandingSoundBySurface(EPhysicalSurface SurfaceType);
	void Landed(const FHitResult& Hit);
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DefaultLandSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* SpinSound;



	/* 달리기 Sprint */
	UFUNCTION(Server, Reliable)
	void ServerStartSprint();
	UFUNCTION(Server, Reliable)
	void ServerStopSprint();
	UFUNCTION(BlueprintCallable)
	bool IsSprinting() const { return bIsSprinting; }
	UPROPERTY(ReplicatedUsing = OnRep_Sprinting, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsSprinting = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_Crouching, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bIsCrouching = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_Guarding, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Combat")
	bool bIsGuarding = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsAiming, VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAiming = false;
	
	/* Double Jump */
	UFUNCTION(Server, Reliable)
	void Server_DoubleJump();
	UFUNCTION(Server, Reliable)
	void Server_DoubleJumpLanded();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_DoubleJumpMontage(bool bIsJump);
	bool CanDoubleJump();

	UPROPERTY(EditDefaultsOnly, Category = "Anim|DoubleJump")
	UAnimMontage* DoubleJumpMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Anim|DoubleJump")
	UAnimMontage* DoubleJumpLandedMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Anim|DoubleJump")
	float DoubleJumpZAmount;

	/* Spin */
	UFUNCTION(BlueprintCallable)
	void StartSpin();
	UFUNCTION(BlueprintCallable)
	void StopSpin();
	UPROPERTY(Replicated)
	bool bShouldSpin;
	float SpinSpeed = 1440.f;
	
	/* Weapon */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeaponType(EWeaponType NewType);
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Weapon")
	EWeaponType CurrentWeaponType;
	
	/* 발소리 관련 Footstep */
	UFUNCTION(Server, Reliable)
	void ServerRequestFootstep(FVector Location, USoundBase* Sound);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayFootstep(FVector Location, USoundBase* Sound);
	UFUNCTION(BlueprintCallable)
	USoundBase* GetFootstepSoundBySurface(EPhysicalSurface SurfaceType);
	
	UPROPERTY(EditDefaultsOnly, Category = "Footstep")
	USoundBase* DefaultFootstepSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> FootstepSounds;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundAttenuation* FootstepAttenuation;

	int32 FootstepSoundIndex = 0; // 순차 재생용 인덱스

protected:
	virtual void BeginPlay() override;
	virtual void AddCharacterAbilities() override;
	virtual void BindToTagChange() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Cost")
	TSubclassOf<UGameplayEffect> GE_SprintManaCost;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Cost")
	TSubclassOf<UGameplayEffect> GE_JumpManaCost;
	UPROPERTY()
	UPRAttributeSet* PRAttributeSet;

	FActiveGameplayEffectHandle ActiveSprintGEHandle;

private:
	virtual void InitAbilityActorInfo() override;
	void SetupStimuliSource();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Equipped Ability Spec Handles
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayAbilitySpecHandle> WeaponAbilityHandles;

	// 상호작용 관련 함수
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	AActor* FindInteractableActor() const;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractionDistance = 100.f;
	
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
	void StartGuard(const FInputActionValue& value);
	UFUNCTION()
	void StopGuard(const FInputActionValue& value);
	void StartAim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);
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
	void OnRep_Guarding();
	UFUNCTION()
	void OnRep_JustJumped();

	void SetJustJumped(bool bNewValue); // 인라인 가능
	void ResetJustJumped();

	// 점프 상태를 일정 시간 후 초기화하기 위한 타이머 핸들
	FTimerHandle JumpResetHandle;

private:
	/* Ability Callback for Input Tag */
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPRInputConfig> InputConfig;
	
	 /*	Clothes Color */
	UFUNCTION()
	void AssignClothesColor();
	UFUNCTION()
	void OnRep_ClothesColor();
	
	UPROPERTY(ReplicatedUsing = OnRep_ClothesColor)
	FLinearColor ClothesColor;
};
