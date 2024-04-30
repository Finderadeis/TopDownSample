#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"
#include <Kismet/GameplayStatics.h>

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

bool AEnemyAIController::IsAlive() const
{
	AEnemyCharacter* ControlledCharacter = Cast<AEnemyCharacter>(GetPawn());
	if (ControlledCharacter == nullptr) return false;
	return ControlledCharacter->IsAlive();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehaviour != nullptr)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		RunBehaviorTree(AIBehaviour);
	}
}
