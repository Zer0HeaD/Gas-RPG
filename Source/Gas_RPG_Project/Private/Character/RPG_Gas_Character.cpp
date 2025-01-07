// Copyright Arthur "Zer0HeaD" Zakirov


#include "Character/RPG_Gas_Character.h"
#include "InputActionValue.h"

// Sets default values
ARPG_Gas_Character::ARPG_Gas_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Weapon = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("Greystone_Sword_Socket"));

	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
