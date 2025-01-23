// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Player_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Player/RPG_PlayerState.h"

#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include <AbilitySystem/RPG_AttributeSet.h>
#include <Player/RPG_PlayerController.h>
#include "UI/HUD/RPG_HUD.h"
#include "UI/Widgets/RPG_UserWidget.h"

ARPG_Player_Character::ARPG_Player_Character()
{

}

void ARPG_Player_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ARPG_Player_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client
	InitAbilityActorInfo();
}

int32 ARPG_Player_Character::GetPlayerLevel()
{
	const ARPG_PlayerState* RPG_PlayerState = GetPlayerState<ARPG_PlayerState>();
	check(RPG_PlayerState);


	return RPG_PlayerState->GetPlayerLevel();
}

void ARPG_Player_Character::InitAbilityActorInfo()
{
	ARPG_PlayerState* RPG_PlayerState = GetPlayerState<ARPG_PlayerState>();
	check(RPG_PlayerState);
	RPG_PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(RPG_PlayerState, this);

	Cast<URPG_AbilitySystemComponent>(RPG_PlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = RPG_PlayerState->GetAbilitySystemComponent();
	AttributeSet = RPG_PlayerState->GetAttributeSet();

	if (ARPG_PlayerController* RPG_PlayerController = Cast<ARPG_PlayerController>(GetController()))
	{
		if (ARPG_HUD* RPG_HUD = Cast<ARPG_HUD>(RPG_PlayerController->GetHUD()))
		{
			RPG_HUD->InitOverlay(RPG_PlayerController, RPG_PlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void ARPG_Player_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ARPG_Player_Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ARPG_Player_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_Player_Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_Player_Character::Look);

		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ARPG_Player_Character::OpenMenu);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARPG_Player_Character::Move(const FInputActionValue& Value)
{
	if (bUIOpened) return;
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPG_Player_Character::Look(const FInputActionValue& Value)
{
	if (bUIOpened) return;
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPG_Player_Character::OpenMenu()
{
	if (ARPG_PlayerController* RPG_PlayerController = Cast<ARPG_PlayerController>(GetController()))
	{
		if (!bUIOpened)
		{
			check(PauseMenuWidgetClass);

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
			InputMode.SetHideCursorDuringCapture(false);

			PauseMenuWidget = CreateWidget<URPG_UserWidget>(RPG_PlayerController, PauseMenuWidgetClass);
			check(PauseMenuWidget);

			InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
			RPG_PlayerController->SetInputMode(InputMode);
			RPG_PlayerController->bShowMouseCursor = true;

			PauseMenuWidget->AddToViewport();
			PauseMenuWidget->SetKeyboardFocus();

			RPG_PlayerController->ConsoleCommand(TEXT("CommonUI.AlwaysShowCursor true"), true);
			RPG_PlayerController->ConsoleCommand(TEXT("CommonUI.EnableGamepadPlatformCurso true"), true);
		}
		else
		{
			FInputModeGameOnly InputMode;
			RPG_PlayerController->SetInputMode(InputMode);
			RPG_PlayerController->bShowMouseCursor = false;

			if (PauseMenuWidget)
			{
				PauseMenuWidget->RemoveFromParent();
			}
		}
		bUIOpened = !bUIOpened;
	}
}

