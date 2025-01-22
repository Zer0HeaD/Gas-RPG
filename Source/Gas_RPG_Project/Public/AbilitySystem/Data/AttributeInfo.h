// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FRPG_AttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FGameplayTag AttributeTag = FGameplayTag();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText AttributeName = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) FText AttributeDescription = FText();

	// ONLY READ! ATTRIBUTE VALUE MUST BE SET IN ANOTHER PLACE!
	UPROPERTY(BlueprintReadOnly) float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	FRPG_AttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TArray<FRPG_AttributeInfo> AttributeInformation;
};
