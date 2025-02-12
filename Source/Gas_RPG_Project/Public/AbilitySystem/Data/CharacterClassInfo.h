// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Knight,
	Warrior,
	Ranger,
	Animal,
	Sorcerer,
	Arkanist,
	Beggar
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults") TSubclassOf<UGameplayEffect> PrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults") TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
};
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults") 
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults") TSubclassOf<UGameplayEffect> SecondaryAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults") TSubclassOf<UGameplayEffect> VitalAttributes;
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults") TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage") TObjectPtr<UCurveTable> DamageCalculationCieffcients;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
