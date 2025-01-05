// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG_Gas_Character.generated.h"

UCLASS(Abstract)
class GAS_RPG_PROJECT_API ARPG_Gas_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPG_Gas_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
