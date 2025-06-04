// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "OneEyedMonster.generated.h"

UCLASS()
class PAYROCK_API AOneEyedMonster : public AEnemyCharacter
{
	GENERATED_BODY()

public:
	AOneEyedMonster();
	UFUNCTION(BlueprintCallable)
	void ToggleTorchLight();
protected:
	virtual void BeginPlay() override;
	void ScheduleNextBlink();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	USpotLightComponent* TorchLight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
	UStaticMeshComponent* TorchBeamVisual;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USphereComponent> WeaponCollision;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FName CollisionSocketName = FName("CollisionSocket");

	FTimerHandle BlinkTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
	float BlinkMin = 0.2f ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blink")
	float BlinkMax = 1.0f ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch|Color")
	FLinearColor NormalLightColor = FLinearColor(1.f, 0.9f, 0.6f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch|Color")
	FLinearColor AlertLightColor = FLinearColor::Red;
};
