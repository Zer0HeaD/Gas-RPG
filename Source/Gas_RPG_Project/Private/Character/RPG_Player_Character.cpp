// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Player_Character.h"
#include "EnhancedInputSubsystems.h"

#include "Player/RPG_PlayerState.h"

#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include <AbilitySystem/RPG_AttributeSet.h>
#include <Player/RPG_PlayerController.h>
#include "UI/HUD/RPG_HUD.h"
#include "UI/Widgets/RPG_UserWidget.h"
#include <Input/RPGInputComponent.h>
#include <AbilitySystemBlueprintLibrary.h>

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "RecoilAnimationComponent.h"
#include "Components/CombatComponent.h"
#include "Character/RPG_ParkourMovementComponent.h"

#include "Kismet/GameplayStatics.h"
#include "RPG_GameplayTags.h"

ARPG_Player_Character::ARPG_Player_Character(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<URPG_ParkourMovementComponent>
		(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	ParkourMovementComponent = Cast<URPG_ParkourMovementComponent>(GetCharacterMovement());
	ParkourMovementComponent->SetIsReplicated(true);

	// Default params to Parkour Movement Component
	ParkourMovementComponent->GravityScale = 1.f;
	ParkourMovementComponent->MaxAcceleration = 1660.f;
	ParkourMovementComponent->BrakingFrictionFactor = 0.15f;
	ParkourMovementComponent->BrakingFriction = 0.15f;
	ParkourMovementComponent->CrouchedHalfHeight = 60.f;
	ParkourMovementComponent->bUseSeparateBrakingFriction = true;
	ParkourMovementComponent->Mass = 200.f;
	ParkourMovementComponent->MaxWalkSpeed = 450.f;
	ParkourMovementComponent->MaxWalkSpeedCrouched = 200.f;
	ParkourMovementComponent->MinAnalogWalkSpeed = 20.f;
	ParkourMovementComponent->BrakingDecelerationWalking = 1024.f;

	ParkourMovementComponent->JumpZVelocity = 500.f;
	ParkourMovementComponent->BrakingDecelerationFalling = 300.f;
	ParkourMovementComponent->AirControl = 0.35f;
	ParkourMovementComponent->AirControlBoostMultiplier = 2.f;
	ParkourMovementComponent->AirControlBoostVelocityThreshold = 25.f;
	ParkourMovementComponent->FallingLateralFriction = 500.f;
	ParkourMovementComponent->PerchRadiusThreshold = 45.f;


	// Create a camera boom (pulls in towards the player if there is a collision)
	FPSCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPS Camera Spring Arm"));
	FPSCameraSpringArm->SetupAttachment(GetMesh(), FName("head"));
	FPSCameraSpringArm->TargetArmLength = 0.f; // The camera follows at this distance behind the character	
	FPSCameraSpringArm->bUsePawnControlRotation = false; // Rotate the arm based on the controller

	// Create a follow camera
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS Camera"));
	FPSCamera->SetupAttachment(FPSCameraSpringArm, USpringArmComponent::SocketName);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	RecoilAnimationComponent = CreateDefaultSubobject<URecoilAnimationComponent>(TEXT("RecoilAnimationComponent"));

	TargetComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Target Component"));
	TargetComponent->SetupAttachment(GetMesh(), "headSocket");
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

void ARPG_Player_Character::Die()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PlayerController->DisableInput(PlayerController);
	}
	Super::Die();
}

int32 ARPG_Player_Character::GetPlayerLevel()
{
	const ARPG_PlayerState* RPG_PlayerState = GetPlayerState<ARPG_PlayerState>();
	check(RPG_PlayerState);


	return RPG_PlayerState->GetPlayerLevel();
}

FWeaponSettings ARPG_Player_Character::GetCurrentWeaponSettings_Implementation()
{
	if (CombatComponent)
	{
		return CombatComponent->GetCurrentWeaponSettings();
	}
	else
	{
		FWeaponSettings settings;
		return settings;
	}
}

UCombatComponent* ARPG_Player_Character::GetCombatComponent_Implementation()
{
	if (CombatComponent)
	{
		return CombatComponent;
	}
	else
	{
		return nullptr;
	}
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

URPG_AbilitySystemComponent* ARPG_Player_Character::GetRPG_ASC()
{
	if(!AbilitySystemComponent) return nullptr;

	if (!RPGAbilitySystemComponent)
	{
		RPGAbilitySystemComponent = Cast<URPG_AbilitySystemComponent>(AbilitySystemComponent);
		/*UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this)*/
	}
	return RPGAbilitySystemComponent;
}

void ARPG_Player_Character::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (CombatComponent && CombatComponent->GetCurrentWeaponSettings().FireMode == EFireMode_PRAS::Semi)
	{
		if (!GetRPG_ASC()) return;
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Open Fire Semi"), true, false, FColor::Cyan, 5.f);
		GetRPG_ASC()->AbilityInputTagPressed(InputTag);
	}
}

void ARPG_Player_Character::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetRPG_ASC()) return;
	GetRPG_ASC()->AbilityInputTagReleased(InputTag);
}

void ARPG_Player_Character::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (CombatComponent && CombatComponent->GetCurrentWeaponSettings().FireMode == EFireMode_PRAS::Auto)
	{
		if (!GetRPG_ASC()) return;
		//UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Open Fire Auto"), true, false, FColor::Cyan, 5.f);
		GetRPG_ASC()->AbilityInputTagHeld(InputTag);
	}
}

FCollisionQueryParams ARPG_Player_Character::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;
	TArray<AActor*> CharacterChildren;
	GetAllChildActors(CharacterChildren);

	Params.AddIgnoredActors(CharacterChildren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ARPG_Player_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ARPG_Player_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator AimRotation = GetControlRotation();

	FRotator Delta = AimRotation - GetActorRotation();
	Delta.Normalize();
	LookRotation = Delta;

	auto AimRot = GetBaseAimRotation();
	auto ActorRot = GetActorRotation();
	float PitchDelta = FRotator::NormalizeAxis(AimRot.Pitch - ActorRot.Pitch);

	PivotOffsetPitch = FMath::GetMappedRangeValueClamped(
		FVector2D(MinWeaponPitch, MaxWeaponPitch),
		FVector2D(MinWeaponPivot, MaxWeaponPivot),
		PitchDelta);
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
	if (URPGInputComponent* RPGInputComponent = CastChecked<URPGInputComponent>(PlayerInputComponent))
	{

		// Jumping assign in blueprint with Vault/Mantle system
		//RPGInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//RPGInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		RPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_Player_Character::Move);

		// Looking
		RPGInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_Player_Character::Look);

		// Open Menu
		RPGInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this, &ARPG_Player_Character::OpenMenu);

		// Sprint
		RPGInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARPG_Player_Character::StartSprinting);

		RPGInputComponent->BindAbilityActions(
			InputConfig, 
			this, 
			&ThisClass::AbilityInputTagPressed, 
			&ThisClass::AbilityInputTagReleased, 
			&ThisClass::AbilityInputTagHeld);
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
		MoveX = MovementVector.X;
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
		LookX = LookAxisVector.X;
		LookY = LookAxisVector.Y;
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

	// STOP RECOIL ON MOVEMENT
	if (CombatComponent && CombatComponent->bIsRecoilRecoveryActive)
	{
		FRotator currentRotation = GetControlRotation();
		FRotator checkpointRotation = CombatComponent->RecoilCheckpoint;

		FRotator deltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(currentRotation, checkpointRotation);

		if (LookAxisVector.Y < 0.f)
		{
			CombatComponent->bIsRecoilRecoveryActive = false;
			CombatComponent->bIsRecoilNeutral = true;
			return;
		}

		if (deltaRotation.Pitch < 0.f)
		{
			CombatComponent->bUpdateRecoilPitchCheckpointInNextShot = true;
		}

		if (LookAxisVector.X != 0.f)
		{
			if (CombatComponent->bIsRecoilYawRecoveryActive)
			{
				CombatComponent->bIsRecoilYawRecoveryActive = false;
			}

			CombatComponent->bUpdateRecoilYawCheckpointInNextShot = true;
		}
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

void ARPG_Player_Character::Jump()
{
	Super::Jump();

	bPressedExtendedJump = true;
	bPressedJump = false;

	if (bIsCrouched)
	{
		UnCrouch();
	}
	StopSprinting();
}

void ARPG_Player_Character::StopJumping()
{
	Super::StopJumping();

	bPressedExtendedJump = false;
}

bool ARPG_Player_Character::CanJumpInternal_Implementation() const
{
	if (Super::CanJumpInternal_Implementation())
	{
		return true;
	}
	else
	{
		return ParkourMovementComponent->IsCrouching() &&
			!ParkourMovementComponent->IsFalling();
	}
}

void ARPG_Player_Character::StartSprinting()
{
	if (ParkourMovementComponent->IsSprinting())
	{
		StopSprinting();
	}
	else
	{
		ParkourMovementComponent->SprintPressed();

		GetWorld()->GetTimerManager().SetTimer(
			SprintCheckTimerHandle,
			this,
			&ARPG_Player_Character::StopSprintingOnMinimalSpeed,
			0.1f,
			true);
	}
}

void ARPG_Player_Character::StopSprinting()
{
	ParkourMovementComponent->SprintReleased();

	GetWorld()->GetTimerManager().ClearTimer(SprintCheckTimerHandle);
}

void ARPG_Player_Character::StopSprintingOnMinimalSpeed()
{
	if (GetVelocity().Length() <= MinimalSpeedToStopSprinting)
	{
		StopSprinting();
	}
}

void ARPG_Player_Character::StartProne()
{
	if (ParkourMovementComponent->IsSprinting())
	{
		StopSprinting();
		ParkourMovementComponent->DolphinDivePressed();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), DolphinDiveDilation);
	}
	else
	{
		ParkourMovementComponent->PronePressed();
	}
}

void ARPG_Player_Character::StopProne()
{
	ParkourMovementComponent->ProneReleased();
}

void ARPG_Player_Character::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (ParkourMovementComponent->IsDolphinDive())
	{
		// apply custom params
		ParkourMovementComponent->GravityScale = 1.f;
		ParkourMovementComponent->BrakingDecelerationFalling = 300.f;
		ParkourMovementComponent->FallingLateralFriction = 500.f;

		ParkourMovementComponent->Safe_bWantsToDolphinDive = false;
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

		if (DolphinDiveLandCameraShake != NULL)
		{
			UGameplayStatics::GetPlayerCameraManager(this, 0)->PlayWorldCameraShake(
				GetWorld(),
				DolphinDiveLandCameraShake,
				GetActorLocation(),
				0.f,
				500.f,
				1.f);
		}

		ParkourMovementComponent->PronePressed();
	}
}

void ARPG_Player_Character::OnAimStarted_Implementation() {}
void ARPG_Player_Character::OnAimEnded_Implementation() {}
