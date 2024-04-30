#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "TopDownCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UStaticMesh;
class UNiagaraSystem;
struct FInputActionValue;
class UHUDWidget;

UCLASS()
class YOUSOMASSESMENT_API ATopDownCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	
protected:
	void Move(const FInputActionValue& Value);
	void LookAt(FVector Target);
	
	void StartShooting();
	void StopShooting();
	void Shoot();

	void QuitGame();

	UPROPERTY(EditAnywhere, Category = Presets)
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = Presets)
	float MovementSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = Presets)
	float SpringArmLength = 1800.0f;

	UPROPERTY(EditAnywhere, Category = Presets)
	FDataTableRowHandle Weapon;

	UPROPERTY(EditAnywhere, Category = Presets)
	TSubclassOf<UHUDWidget> HUDWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* QuitAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

	APlayerController* PlayerController;

	FTimerHandle ShootTimerHandle;
	UPROPERTY()
	UHUDWidget* HUD;
};
