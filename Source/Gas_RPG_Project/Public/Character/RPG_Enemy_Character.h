// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Character/RPG_Gas_Character.h"
#include "RPG_Enemy_Character.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_Enemy_Character : public ARPG_Gas_Character
{
	GENERATED_BODY()
public:
	ARPG_Enemy_Character();

protected:
	virtual void BeginPlay() override;

private:
	void InitAbilityActorInfo() override;
};
