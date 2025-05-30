// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PayRock/Item/PRItemEnum.h"
#include "PRCharacter.generated.h"

class USphereComponent;
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

	virtual void Die(FVector HitDirection = FVector::ZeroVector) override;

	

	// SpringArm Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	// Aiming 카메라 전용 설정
	float DefaultArmLength;
	float AimingArmLength;

	//관전용 카메라 컨트롤 Replication
	UPROPERTY(Replicated)
	FRotator ReplicatedControlRotation;


	FVector DefaultSocketOffset = FVector::ZeroVector;
	FVector AimingSocketOffset = FVector(0.f, 50.f, 30.f); // 오른쪽 위에서 보는 느낌

	float CameraInterpSpeed;

	UPROPERTY(ReplicatedUsing = OnRep_MaxWalkSpeed)
	float ReplicatedMaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float CurrentTargetSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Interp")
	float CurrentInterpRate;

	// Fist Collision Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	USphereComponent* LeftHandCollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CollisionComponent")
	USphereComponent* RightHandCollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* Weapon2;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName Weapon2SocketName = FName("Weapon2Socket");

	// Move Speed Property
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


	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayAbility> GA_UseHealItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blessing")
	class UBlessingComponent* BlessingComponent;

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

	UPROPERTY(ReplicatedUsing = OnRep_JustJumped, VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	bool bJustJumped = false;

	UPROPERTY(ReplicatedUsing = OnRep_IsAiming, VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsAiming = false;

	UFUNCTION(BlueprintCallable, Category = "Anim|Movement")
	float GetLastJumpDirection() const { return LastJumpDirection; }

	bool bResetJustJumpedNextFrame = false;
	float JustJumpedElapsedTime = 0.f;

	UFUNCTION()
	void OnRep_MaxWalkSpeed();

	void SetSpeedMode(bool bSprintState);
	void SetCrouchSpeed();
	void ResetToWalkSpeed();

	UFUNCTION(Server, Reliable)
	void ServerStartJump();

	UFUNCTION(Server, Reliable)
	void ServerStartSprint();

	UFUNCTION(Server, Reliable)
	void ServerStopSprint();

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bNewAiming);

	// 마지막 점프 시점의 이동 방향
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim|Movement")
	float LastJumpDirection = 0.f;

	UFUNCTION()
	void OnRep_IsAiming();

	// 기존
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Weapon")
	EWeaponType CurrentWeaponType;

	void SetWeaponType(EWeaponType NewType);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	EWeaponType GetCurrentWeaponType() const { return CurrentWeaponType; }

	// 발소리 관련
	UFUNCTION(Server, Reliable)
	void ServerRequestFootstep(FVector Location, USoundBase* Sound);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayFootstep(FVector Location, USoundBase* Sound);

	UFUNCTION(BlueprintCallable)
	USoundBase* GetFootstepSoundBySurface(EPhysicalSurface SurfaceType);

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundBase*> FootstepSounds;

	int32 FootstepSoundIndex = 0; // 순차 재생용 인덱스

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundAttenuation* FootstepAttenuation;


	// 발소리 사운드 큐
	UPROPERTY(EditDefaultsOnly, Category = "Footstep")
	USoundBase* DefaultFootstepSound;

	// 착지
	UFUNCTION(Server, Reliable)
	void ServerRequestLandingSound(FVector Location, USoundBase* Sound);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayLandingSound(FVector Location, USoundBase* Sound);

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* DefaultLandSound;

	USoundBase* GetLandingSoundBySurface(EPhysicalSurface SurfaceType);

	void Landed(const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	virtual void AddCharacterAbilities() override;
	
private:
	virtual void InitAbilityActorInfo() override;
	void SetupStimuliSource();

protected:
	// Equipped Ability Spec Handles
	UPROPERTY(BlueprintReadWrite)
	TArray<FGameplayAbilitySpecHandle> WeaponAbilityHandles;
	
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
	void OnRep_Attacking();

	UFUNCTION()
	void OnRep_Guarding();

	UFUNCTION()
	void OnRep_JustJumped();

	void SetJustJumped(bool bNewValue); // 인라인 가능

	void ResetJustJumped();

	// 점프 상태를 일정 시간 후 초기화하기 위한 타이머 핸들
	FTimerHandle JumpResetHandle;


private:
	/* Callback functions for binding ability input actions based on Input Tags  */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UPRInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
