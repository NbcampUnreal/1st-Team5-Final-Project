// PayRockGames


#include "BaseItem.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseItem::UsingEffect()
{
	
}

FString ABaseItem::GetName() const
{
	return ItemName;
}

TSubclassOf<UGameplayAbility> ABaseItem::GetAbilities() const
{
	return AbilityClass;
}

float ABaseItem::GetAbilityLevel() const
{
	return AbilityLevel;
}

int32 ABaseItem::GetItemPrice() const
{
	return ItemPrice;
}


