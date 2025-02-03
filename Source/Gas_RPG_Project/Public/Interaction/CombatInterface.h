// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
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

};
