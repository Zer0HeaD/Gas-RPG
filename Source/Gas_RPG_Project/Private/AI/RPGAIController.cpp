// Copyright Arthur "Zer0HeaD" Zakirov


#include "AI/RPGAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ARPGAIController::ARPGAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>("Blackboard Component");
	check(BlackboardComponent);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("Behavior Tree Component");
	check(BehaviorTreeComponent);
}
