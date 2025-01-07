// Copyright Arthur "Zer0HeaD" Zakirov


#include "Player/RPG_PlayerController.h"
#include "../../../../../../epic/UE_5.5/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../epic/UE_5.5/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

ARPG_PlayerController::ARPG_PlayerController()
{
	bReplicates = true;
}

void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set input mode to game only by default
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}


