#include "Boss3DTextActor.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ABoss3DTextActor::ABoss3DTextActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootSceneComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetDrawSize(FVector2D(300.f, 100.f));
	WidgetComponent->SetTwoSided(true);
	WidgetComponent->SetVisibility(true);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetRelativeRotation(FRotator::ZeroRotator);
}

void ABoss3DTextActor::BeginPlay()
{
	Super::BeginPlay();
	
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Yaw += 180.f;
	SetActorRotation(CurrentRotation);

	InitialScale = GetActorScale3D();

	if (WidgetComponent && WidgetComponent->GetWidget())
	{
		WidgetComponent->InitWidget();
	}

	if (NiagaraComponent)
	{
		NiagaraComponent->Activate(true);
	}

	MoveSpeed = 600.f / FadeDuration;

	GetWorld()->GetTimerManager().SetTimer(
		EffectTimerHandle,
		this,
		&ABoss3DTextActor::UpdateEffect,
		UpdateInterval,
		true
	);
}


void ABoss3DTextActor::UpdateEffect()
{
	ElapsedTime += UpdateInterval;

	const float T = ElapsedTime / FadeDuration;
	const float Alpha = FMath::Clamp(FMath::Pow(1.0f - T, 2.0f), 0.0f, 1.0f);

	if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
	{
		if (UTextBlock* TextBlock = Cast<UTextBlock>(Widget->GetWidgetFromName(TEXT("TextBlock_Notice"))))
		{
			FLinearColor Color = TextBlock->ColorAndOpacity.GetSpecifiedColor();
			Color.A = Alpha;
			TextBlock->SetColorAndOpacity(Color);
		}
	}

	AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * UpdateInterval);

	const float ScaleAlpha = FMath::Clamp(T, 0.0f, 1.0f);
	SetActorScale3D(FMath::Lerp(InitialScale, TargetScale, ScaleAlpha));

	if (ElapsedTime >= FadeDuration)
	{
		Destroy();
	}
}
