// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "RPG_AbilitySystemLibrary.generated.h"

class URPG_OverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API URPG_AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "RPG_AbilitySystemLibrary|WidgetController") 
	static URPG_OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "RPG_AbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "RPG_AbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefultAttributes(
		const UObject* WorldContextObject, 
		ECharacterClass CharacterClass, 
		float Level,
		UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "RPG_AbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(
		const UObject* WorldContextObject,
		UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "RPG_AbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
};
