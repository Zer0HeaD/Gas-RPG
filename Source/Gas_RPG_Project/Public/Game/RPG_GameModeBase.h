// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPG_GameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults") 
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
