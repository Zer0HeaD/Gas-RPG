// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/RPG_GameplayAbility.h"
#include "RPG_SummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_SummonAbility : public URPG_GameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable) TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditDefaultsOnly, Category = "Summoning") int32 NumMinions = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Summoning") TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning") float MinSpawnDistance = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Summoning") float MaxSpawnDistance = 250.f;
	UPROPERTY(EditDefaultsOnly, Category = "Summoning") float SpawnSpread = 90.f;

	UFUNCTION(BlueprintPure, Category = "Summoning") TSubclassOf<APawn> GetRandomMinionClass();
};
