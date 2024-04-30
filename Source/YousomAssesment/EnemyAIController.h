#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class YOUSOMASSESMENT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	bool IsAlive() const;

protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehaviour;
};
