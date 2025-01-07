// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPG_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARPG_PlayerController();

protected:
	virtual void BeginPlay() override;

private:
};
