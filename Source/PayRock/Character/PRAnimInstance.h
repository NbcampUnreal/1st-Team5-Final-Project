// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PRAnimInstance.generated.h"

class APRCharacter;

UCLASS()
class PAYROCK_API UPRAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    // 현재 소유 캐릭터
    UPROPERTY(EditAnywhere, Category = "Anim|Character")
    APRCharacter* OwnerCharacter;

    // 캐릭터 상태들 (AnimBP에서 사용)
    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    float Speed;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    float MoveDirection;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    float ZVelocity;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    bool bIsSprinting;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    bool bShouldMove;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    bool bIsCrouching;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    bool bIsInAir;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Combat")
    bool bIsAttacking;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Combat")
    bool bIsGuarding;

    UPROPERTY(BlueprintReadWrite, Category = "Anim|Movement")
    bool bJustJumped;

    // 이동 방향 기반 골반 회전 오프셋
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float PelvisYawOffset = 0.f;
};
