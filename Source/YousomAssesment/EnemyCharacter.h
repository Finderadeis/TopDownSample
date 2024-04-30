
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class YOUSOMASSESMENT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsAlive() const;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void ApplyDamage(AActor* TargetActor);

protected:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 20.0f;
	UPROPERTY(VisibleAnywhere)
	float CurrentHealth = MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* DamageTriggerRange;
	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;
	UPROPERTY(EditDefaultsOnly)
	float DamageRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraSystem* BloodVFX;

	struct FTimerHandle DamageTimerHandle;
};
