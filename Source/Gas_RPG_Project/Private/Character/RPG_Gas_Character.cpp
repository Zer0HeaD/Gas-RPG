// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Gas_Character.h"
#include "InputActionValue.h"
#include "AbilitySystem/RPG_AbilitySystemComponent.h"
#include "Player/RPG_PlayerController.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ARPG_Gas_Character::ARPG_Gas_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("Greystone_Sword_Socket"));

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//GetMesh()->VisibilityBasedAnimTickOption(EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones);
}

UAbilitySystemComponent* ARPG_Gas_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ARPG_Gas_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ARPG_Gas_Character::InitAbilityActorInfo()
{
}

FVector ARPG_Gas_Character::GetCombatSocketLocation()
{
	check(Weapon);
	return 	GetMesh()->GetSocketLocation(WeaponTipSocketName);
}

FRotator ARPG_Gas_Character::GetOwnerRotation()
{
	return bUseControllerRotationYaw ? GetControlRotation() : GetActorRotation();
}

void ARPG_Gas_Character::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle =
	GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ARPG_Gas_Character::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

// EXECUTE ONLY ON A SERVER
void ARPG_Gas_Character::AddCharacterAbilities()
{
	URPG_AbilitySystemComponent* ASC = CastChecked<URPG_AbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASC->AddCharacterAbilities(StartupAbilities);
}


