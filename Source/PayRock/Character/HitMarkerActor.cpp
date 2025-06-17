#include "HitMarkerActor.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/OutputDeviceNull.h"

AHitMarkerActor::AHitMarkerActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HitMarkerWidget"));
	RootComponent = WidgetComponent;

	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);  // or World
	WidgetComponent->SetDrawSize(FVector2D(64.f, 64.f));
	WidgetComponent->SetTwoSided(true);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AHitMarkerActor::BeginPlay()
{
	Super::BeginPlay();

	if (HitMarkerWidgetClass)
	{
		WidgetComponent->SetWidgetClass(HitMarkerWidgetClass);
		WidgetComponent->InitWidget();

		if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
		{
			static const FString FuncName = TEXT("PlayHitMarker");
			FOutputDeviceNull ar;
			Widget->CallFunctionByNameWithArguments(*FuncName, ar, nullptr, true);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AActor::K2_DestroyActor, 0.4f, false);
}
