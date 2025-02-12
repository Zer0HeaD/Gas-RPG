// Copyright Arthur "Zer0HeaD" Zakirov

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RPGAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class GAS_RPG_PROJECT_API ARPGAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARPGAIController();

protected:
	UPROPERTY() TObjectPtr<UBlackboardComponent> BlackboardComponent;
	UPROPERTY() TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

};
