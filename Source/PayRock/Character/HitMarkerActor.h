#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "HitMarkerActor.generated.h"

UCLASS()
class PAYROCK_API AHitMarkerActor : public AActor
{
	GENERATED_BODY()

public:
	AHitMarkerActor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "HitMarker")
	TSubclassOf<UUserWidget> HitMarkerWidgetClass;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitMarker", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetComponent;

	FTimerHandle DestroyTimerHandle;
};
