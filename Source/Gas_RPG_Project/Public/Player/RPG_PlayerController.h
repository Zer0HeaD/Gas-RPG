// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPG_PlayerController.generated.h"

class UDamageTextComponent;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ARPG_PlayerController();

	UFUNCTION(Client, Reliable) void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
