// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RPG_InputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FRPGInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly) const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly) FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	const UInputAction* FindAbilityInputActionForTag(
		const FGameplayTag& InputTag, 
		bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<FRPGInputAction> AbilityInputActions;
};
