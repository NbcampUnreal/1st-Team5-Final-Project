// PayRockGames

#include "PRAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "PayRock/PRGameplayTags.h"
#include "PayRock/Character/BaseCharacter.h"
#include "Abilities/Blessing/BaseBlessingAbility.h"
#include "Kismet/GameplayStatics.h"
#include "PayRock/Enemy/EnemyCharacter.h"
#include "PayRock/GameSystem/PRAdvancedGameInstance.h"

UPRAttributeSet::UPRAttributeSet()
{
}

void UPRAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* Primary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Spirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Luck, COND_None, REPNOTIFY_Always);

	/* Secondary Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CriticalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, DebuffResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, LootQualityModifier, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, CarryWeight, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, BonusDamage, COND_None, REPNOTIFY_Always);
	
	/* Vital Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPRAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UPRAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UPRAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		/*const float HealthCurrent = GetHealth();
		const float HealthBase = GetOwningAbilitySystemComponent()->GetNumericAttributeBase(GetHealthAttribute());
		UE_LOG(LogTemp, Warning, TEXT(
			"[Health] AvatarActor: %s / Amount: %f / Health(Current): %f / Health(Base): %f"),
			*GetOwningAbilitySystemComponent()->GetAvatarActor()->GetName(),
			Data.EvaluatedData.Magnitude, HealthCurrent, HealthBase);*/
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));

		/*const float ManaCurrent = GetMana();
		const float ManaBase = GetOwningAbilitySystemComponent()->GetNumericAttributeBase(GetManaAttribute());
		UE_LOG(LogTemp, Warning, TEXT(
			"[Mana] AvatarActor: %s / Amount: %f / Mana(Current): %f / Mana(Base): %f"),
			*GetOwningAbilitySystemComponent()->GetAvatarActor()->GetName(),
			Data.EvaluatedData.Magnitude, ManaCurrent, ManaBase);*/
	}
	else if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float Damage = HandleIncomingDamage(Props, Data);
		HandleLifeSteal(Props, Damage);
		HandleAutoCounter(Props, Damage);
	}
}

float UPRAttributeSet::HandleIncomingDamage(const FEffectProperties& Props, const FGameplayEffectModCallbackData& Data)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	if (LocalIncomingDamage <= 0.f) return 0.f;

	// Activate Hit React
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FPRGameplayTags::Get().Effects_HitReact);
	if (Props.TargetASC)
	{
		Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);	
	}
	
	SetIncomingDamage(0.f);
	const float CalculatedDamage = GetCalculatedDamage(LocalIncomingDamage, Props);
	if (CalculatedDamage > 0.f)
	{
		float NewHealth = FMath::Clamp(GetHealth() - CalculatedDamage, 0.f, GetMaxHealth());

		// Invincible Buff (e.g. YiSunSin Blessing)
		if (NewHealth <= 0.f &&
			GetOwningAbilitySystemComponent()->HasMatchingGameplayTag(FPRGameplayTags::Get().Status_Buff_Invincible))
		{
			NewHealth = 1.f;
		}
		
		SetHealth(NewHealth);
		
		UE_LOG(LogTemp, Warning, TEXT(
			"[DAMAGE] From: %s / To: %s / IncomingDamage: %f / CalculatedDamage: %f / GetHealth: %f / GetMaxHealth: %f"),
			*Props.SourceAvatarActor->GetName(),
			*GetOwningAbilitySystemComponent()->GetAvatarActor()->GetName(),
			LocalIncomingDamage, CalculatedDamage, GetHealth(), GetMaxHealth());

		if (NewHealth <= 0.f)
		{
			// Handle death
			ABaseCharacter* Character = Cast<ABaseCharacter>(Props.TargetCharacter);

			if (Character && Props.SourceController)
			{
				AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Character);
				if (Enemy)
				{
					Enemy->LastHitInstigator = Props.SourceController; 
				}
			}
			
			if (Character && Character->HasAuthority() && Props.SourceCharacter)
			{
				Character->Die((Props.TargetCharacter->GetActorLocation() - Props.SourceCharacter->GetActorLocation()).GetSafeNormal());
			}

			
		}
	}
	return CalculatedDamage;
}

float UPRAttributeSet::GetCalculatedDamage(float LocalIncomingDamage, const FEffectProperties& Props)
{
	if (!Props.SourceASC || LocalIncomingDamage <= 0.f) return 0.f;
	
	const UPRAttributeSet* AttackerAttributeSet =
		Cast<UPRAttributeSet>(Props.SourceASC->GetAttributeSet(UPRAttributeSet::StaticClass()));

	float NetArmor = GetArmor();
	float CriticalMultiplier = 1.f;
	if (AttackerAttributeSet)
	{
		// Critical Hit Multiplier
		const float EffectiveCritChance = FMath::Clamp(
			AttackerAttributeSet->GetCriticalChance() - GetCriticalResistance(), 0.f, 100.f);
		const bool bIsCritical = FMath::RandRange(0.f, 100.f) < EffectiveCritChance;
		if (bIsCritical) UE_LOG(LogTemp, Warning, TEXT("Critical Hit!"));
		CriticalMultiplier = bIsCritical ? (AttackerAttributeSet->GetCriticalDamage() / 100.f) : 1.f;

		// Net Armor = Defender Armor - Attacker Armor Penetration
		NetArmor = FMath::Max(0.f, GetArmor() - AttackerAttributeSet->GetArmorPenetration());

		// Attacker's Strength multiplier
		const float StrengthMultiplier = 1.f + (AttackerAttributeSet->GetStrength() / 500.f);
		LocalIncomingDamage *= StrengthMultiplier;

		// Attacker's Bonus Damage multiplier (percentage)
		const float BonusDamageMultiplier = 1.f + AttackerAttributeSet->GetBonusDamage() / 100.f;
		LocalIncomingDamage *= BonusDamageMultiplier;
	}
	
	// Armor Reduction (percentage)
	const float ArmorReduction = NetArmor / (NetArmor + 100.f);
	
	const float DamageAfterArmor = LocalIncomingDamage * (1.f - ArmorReduction);

	const float FinalDamage = DamageAfterArmor * CriticalMultiplier;
	
	return FinalDamage;
}

void UPRAttributeSet::HandleLifeSteal(const FEffectProperties& Props, float DealtDamage)
{
	if (DealtDamage <= 0.f) return;
	
	FGameplayTag LifeStealTag = FPRGameplayTags::Get().Status_Buff_LifeSteal;
	if (!Props.SourceASC || !Props.SourceASC->HasMatchingGameplayTag(LifeStealTag)) return;

	int32 LifeStealLevel = 1;
	TSubclassOf<UGameplayEffect> LifeStealEffectClass = nullptr;

	FGameplayTagContainer Tags;
	Tags.AddTag(LifeStealTag);
	for (const auto& ActiveEffect : Props.SourceASC->GetActiveEffectsWithAllTags(Tags))
	{
		if (!ActiveEffect.WasSuccessfullyApplied()) continue;
		
		// will return 1 if for some reason data isn't valid
		FGameplayEffectContextHandle ContextHandle = Props.SourceASC->GetEffectContextFromActiveGEHandle(ActiveEffect);
		LifeStealLevel = ContextHandle.GetAbilityLevel();
		if (const UBaseBlessingAbility* LifeStealAbility =
			Cast<UBaseBlessingAbility>(ContextHandle.GetAbilityInstance_NotReplicated()))
		{
			if (!IsValid(LifeStealAbility->LifeStealEffectClass)) continue;
			LifeStealEffectClass = LifeStealAbility->LifeStealEffectClass;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LifeSteal Buff is applied, but casting GetAbilityInstance_NotReplicated() failed"));
			continue;
		}
		break; // Presuming there's only one GA that granted this buff
	}

	if (IsValid(LifeStealEffectClass))
	{
		FGameplayEffectContextHandle ContextHandle = Props.SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = Props.SourceASC->MakeOutgoingSpec(
			LifeStealEffectClass, LifeStealLevel, ContextHandle);
		// TODO : make this generic for different buffs
		float LifeStealMultiplier = 0.2f + static_cast<float>(LifeStealLevel) * 0.1f;
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
			SpecHandle, FPRGameplayTags::Get().Status_Buff_LifeSteal,
			LifeStealMultiplier * DealtDamage);
		Props.SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LifeSteal Buff is applied, but the LifeStealEffectClass was not set in the attack ability"))
	}
}

void UPRAttributeSet::HandleAutoCounter(const FEffectProperties& Props, float DealtDamage)
{
	if (DealtDamage <= 0.f) return;

	FGameplayTag AutoCounterTag = FPRGameplayTags::Get().Status_Buff_AutoCounter;
	if (Props.TargetASC->HasMatchingGameplayTag(AutoCounterTag))
	{
		FGameplayTagContainer Tags;
		Tags.AddTagFast(AutoCounterTag);
		if (!Props.TargetASC->TryActivateAbilitiesByTag(Tags))
		{
			UE_LOG(LogTemp, Warning, TEXT("AutoCounter Buff is applied, but failed to activate ability"))
		}
	}
}

void UPRAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid()
		&& Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && IsValid(Props.SourceAvatarActor))
		{
			if (APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Props.SourceController->GetCharacter();
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

/* Primary Attributes */

void UPRAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Strength, OldStrength);
}

void UPRAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Intelligence, OldIntelligence);
}

void UPRAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Dexterity, OldDexterity);
}

void UPRAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Vitality, OldVitality);
}

void UPRAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Spirit, OldSpirit);
}

void UPRAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Luck, OldLuck);
}

/* Secondary Attributes */

void UPRAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Armor, OldArmor);
}

void UPRAttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CriticalResistance, OldCriticalResistance);
}

void UPRAttributeSet::OnRep_DebuffResistance(const FGameplayAttributeData& OldDebuffResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, DebuffResistance, OldDebuffResistance);
}

void UPRAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, MaxHealth, OldMaxHealth);
}

void UPRAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, MaxMana, OldMaxMana);
}

void UPRAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UPRAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UPRAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UPRAttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CriticalChance, OldCriticalChance);
}

void UPRAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CriticalDamage, OldCriticalDamage);
}

void UPRAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UPRAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, AttackSpeed, OldAttackSpeed);
}

void UPRAttributeSet::OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CooldownReduction, OldCooldownReduction);
}

void UPRAttributeSet::OnRep_LootQualityModifier(const FGameplayAttributeData& OldLootQualityModifier) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, LootQualityModifier, OldLootQualityModifier);
}

void UPRAttributeSet::OnRep_CarryWeight(const FGameplayAttributeData& OldCarryWeight) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, CarryWeight, OldCarryWeight)
}

void UPRAttributeSet::OnRep_BonusDamage(const FGameplayAttributeData& OldBonusDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, BonusDamage, OldBonusDamage);
}

/* Vital Attributes */

void UPRAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Health, OldHealth);
}

void UPRAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPRAttributeSet, Mana, OldMana);	
}

