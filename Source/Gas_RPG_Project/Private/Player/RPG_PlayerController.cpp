// Copyright Arthur "Zer0HeaD" Zakirov


#include "Player/RPG_PlayerController.h"
#include "../../../../../../epic/UE_5.5/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../../../epic/UE_5.5/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "UI/Widgets/DamageTextComponent.h"

ARPG_PlayerController::ARPG_PlayerController()
{
	bReplicates = true;
}

void ARPG_PlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set input mode to game only by default
	FInputModeGameOnly InputModeGameOnly;
	SetInputMode(InputModeGameOnly);
}


