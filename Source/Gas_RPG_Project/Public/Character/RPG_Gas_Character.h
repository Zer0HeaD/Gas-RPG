// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "RPG_Gas_Character.generated.h"



class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class GAS_RPG_PROJECT_API ARPG_Gas_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPG_Gas_Character();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY() TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY() TObjectPtr<UAttributeSet> AttributeSet;


	UPROPERTY(EditAnywhere, Category = "Combat") TObjectPtr<UStaticMeshComponent> Weapon;

public:
};
