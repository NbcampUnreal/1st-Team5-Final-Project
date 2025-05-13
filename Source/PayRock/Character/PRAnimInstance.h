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
    // ���� ���� ĳ����
    UPROPERTY(EditAnywhere, Category = "Anim|Character")
    APRCharacter* OwnerCharacter;

    // ĳ���� ���µ� (AnimBP���� ���)
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

    // �̵� ���� ��� ��� ȸ�� ������
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
    float PelvisYawOffset = 0.f;
};
