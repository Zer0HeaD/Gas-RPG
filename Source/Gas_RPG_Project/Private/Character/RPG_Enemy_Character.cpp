// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Enemy_Character.h"
#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include <AbilitySystem/RPG_AttributeSet.h>
#include "Components/WidgetComponent.h"
#include "AbilitySystem/RPG_AbilitySystemLibrary.h"
#include "RPG_GameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/RPGAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ARPG_Enemy_Character::ARPG_Enemy_Character(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)  // Make sure to call the base class constructor
{
	AbilitySystemComponent = CreateDefaultSubobject<URPG_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<URPG_AttributeSet>("AttributeSet");

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>("Stat Widget");
	InfoWidget->SetupAttachment(GetRootComponent());

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ARPG_Enemy_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// USE IN MULTIPLAYER CASE ONLY!
	//if (!HasAuthority()) return;
	RPGAIController = Cast<ARPGAIController>(NewController);
	RPGAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	RPGAIController->RunBehaviorTree(BehaviorTree);
	RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangeAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void ARPG_Enemy_Character::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	//OutLocation = GetMesh()->GetSocketLocation("HeadSocket");
	//OutRotation = GetViewRotation();
	Location = GetMesh()->GetSocketLocation("HeadSocket");
	Rotation.Yaw += GetMesh()->GetSocketTransform("HeadSocket", RTS_World).Rotator().Yaw;
}

void ARPG_Enemy_Character::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	if (HasAuthority())
	{
		URPG_AbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (URPG_UserWidget* RPG_UserWidget = Cast<URPG_UserWidget>(InfoWidget->GetUserWidgetObject()))
	{
		RPG_UserWidget->SetWidgetController(this);
	}


	BindDelegates();
}

void ARPG_Enemy_Character::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<URPG_AbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void ARPG_Enemy_Character::InitializeDefaultAttributes() const
{
	URPG_AbilitySystemLibrary::InitializeDefultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ARPG_Enemy_Character::BindDelegates()
{
	if (const URPG_AttributeSet* RPG_AS = Cast<URPG_AttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPG_AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(RPG_AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(
			FRPG_GameplayTags::Get().Effects_HitReact, 
			EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &ARPG_Enemy_Character::HitReactTagChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(
			FRPG_GameplayTags::Get().Effects_HeavyHitReact,
			EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &ARPG_Enemy_Character::HeavyHitReactTagChanged);

		OnHealthChanged.Broadcast(RPG_AS->GetHealth());
		OnMaxHealthChanged.Broadcast(RPG_AS->GetMaxHealth());
	}
}

int32 ARPG_Enemy_Character::GetPlayerLevel()
{
	return Level;
}

void ARPG_Enemy_Character::Die()
{
	//TODO: we can set lifespan to delete enemy from world with dessolving, or leave it in ragdoll.
	SetLifeSpan(LifeSpan);
	if (RPGAIController) RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);

	InfoWidget->DestroyComponent(true);
	Super::Die();
}

void ARPG_Enemy_Character::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? HitWalkSpeed : BaseWalkSpeed;
	RPGAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);

	if (bHitReacting)
	{
		//TODO: create more efficient code to rotate enemy, current code works only in singleplayer
		if (APawn* player = UGameplayStatics::GetPlayerPawn(this, 0))
		{
			FVector PlayerDirection = (player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			PlayerDirection.Z = 0.f;
			FRotator TargetRotation = PlayerDirection.Rotation();
			TargetRotation.Pitch = 0.f;
			TargetRotation.Roll = 0.f;
			SetActorRotation(TargetRotation);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString("FAILED TO GET A PLAYER PAWN FOR ROTATION ENEMY TO PLAYER"));
		}

		LaunchCharacter(-GetActorForwardVector() * KnockbackImpulseForce, true, false);
	}
}

void ARPG_Enemy_Character::HeavyHitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->StopMovementImmediately();
}
