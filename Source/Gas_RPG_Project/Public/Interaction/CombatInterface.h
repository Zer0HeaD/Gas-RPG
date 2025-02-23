// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Components/CombatComponent.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
class UCombatComponent;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) UAnimMontage* Montage = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTag MontageTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAS_RPG_PROJECT_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();
	virtual FRotator GetOwnerRotation();
	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) UAnimMontage* GetHitReactMontage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) UAnimMontage* GetDeathMontage();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) UNiagaraSystem* GetHitEffect();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) USoundBase* GetHitSound();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) int32 GetMinionCount();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void SetMinionCount(int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) FWeaponSettings GetCurrentWeaponSettings();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) UCombatComponent* GetCombatComponent();

};
