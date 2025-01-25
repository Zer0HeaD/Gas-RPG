// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "Character/RPG_Gas_Character.h"
#include "UI/Widgets/RPG_UserWidget.h"
#include "GameplayTagContainer.h"
#include <AbilitySystem/RPG_AbilitySystemComponent.h>
#include "RPG_Player_Character.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class URPG_InputConfig;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPG_Player_Character : public ARPG_Gas_Character
{
	GENERATED_BODY()
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Open Menu Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MenuAction;

public:
	ARPG_Player_Character();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

#pragma region Combat Interface

	virtual int32 GetPlayerLevel() override;

#pragma endregion

protected:
	virtual void BeginPlay() override;

	virtual void NotifyControllerChanged() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void OpenMenu();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player's Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<URPG_UserWidget> PauseMenuWidgetClass;

	UPROPERTY() TObjectPtr<URPG_UserWidget> PauseMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) bool bUIOpened = false;

private:
	void InitAbilityActorInfo() override;

	URPG_AbilitySystemComponent* GetRPG_ASC();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<URPG_InputConfig> InputConfig;

	UPROPERTY() TObjectPtr<URPG_AbilitySystemComponent> RPGAbilitySystemComponent;

public:


};
