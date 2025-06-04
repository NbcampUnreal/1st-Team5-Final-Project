#pragma once

#include "CoreMinimal.h"
#include "EBossPhase.generated.h"

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1 UMETA(DisplayName = "Phase1"),
	Phase2 UMETA(DisplayName = "Phase2"),
	Phase3 UMETA(DisplayName = "Phase3")
};