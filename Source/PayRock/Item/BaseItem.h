// PayRockGames

#pragma once

#include "CoreMinimal.h"
#include "PRItemInterface.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class PAYROCK_API ABaseItem : public AActor, public IPRItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> ActiveAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS")
	TSubclassOf<UGameplayAbility> PassiveAbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	int32 AbilityLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FString ItemName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	int32 ItemPrice;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ABaseItem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual FString GetName() const override;

	virtual TSubclassOf<UGameplayAbility> GetAbilities() const override;
	
	virtual int32 GetAbilityLevel() const override;
	
	virtual int32 GetItemPrice() const override;
};
