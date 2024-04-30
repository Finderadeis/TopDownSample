#include "TopDownCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include <NiagaraFunctionLibrary.h>
#include "Blueprint/UserWidget.h"
#include "HUDWidget.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/DamageEvents.h"
#include "SWeaponData.h"
#include "TopDownGameMode.h"

ATopDownCharacter::ATopDownCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = SpringArmLength;
	SpringArm->bUsePawnControlRotation = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("WeaponSocket"));
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	
	CurrentHealth = MaxHealth;

	FWeaponData* WeaponData = Weapon.GetRow<FWeaponData>(TEXT("Mesh"));
	WeaponMesh->SetStaticMesh(WeaponData->Mesh);

	HUD = CreateWidget<UHUDWidget>(GetWorld(), HUDWidget);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
		HUD->SetHealthBarPercent(GetHealthPercent());
		ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->SetPlayerHUD(HUD);
	}
}

void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		LookAt(HitResult.Location);
	}
}

void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATopDownCharacter::StartShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ATopDownCharacter::StopShooting);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::Move);
		EnhancedInputComponent->BindAction(QuitAction, ETriggerEvent::Triggered, this, &ATopDownCharacter::QuitGame);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' Failed to find EnhancedInputComponent!"), *GetNameSafe(this));
	}
}

float ATopDownCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CurrentHealth <= 0)
	{
		return 0.0f;
	}
	float RetVal = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHealth = CurrentHealth - RetVal;

	HUD->SetHealthBarPercent(GetHealthPercent());

	if (CurrentHealth <= 0)
	{
		ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->PawnKilled(this);
		this->DisableInput(PlayerController);
	}

	return RetVal;
}

float ATopDownCharacter::GetHealthPercent() const
{
	return CurrentHealth/MaxHealth;
}

void ATopDownCharacter::StartShooting()
{
	FWeaponData* WeaponData = Weapon.GetRow<FWeaponData>(TEXT("FireRate"));
	GetWorld()->GetTimerManager().SetTimer(ShootTimerHandle, this, &ATopDownCharacter::Shoot, WeaponData->FireRate, true);
}

void ATopDownCharacter::StopShooting()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootTimerHandle);
}

void ATopDownCharacter::Shoot()
{
	FWeaponData* WeaponData = Weapon.GetRow<FWeaponData>(TEXT("MuzzleFlash"));
	UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponData->MuzzleFlash, WeaponMesh, TEXT("Muzzle"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);

	FHitResult HitResult;
	FVector ShotDirection;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	WeaponData = Weapon.GetRow<FWeaponData>(TEXT("Range"));

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WeaponMesh->GetSocketLocation(TEXT("Muzzle")), GetActorLocation() + GetActorForwardVector() * WeaponData->Range, ECC_WorldDynamic, CollisionParams);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor != nullptr)
		{
			WeaponData = Weapon.GetRow<FWeaponData>(TEXT("Damage"));
			FPointDamageEvent DamageEvent(WeaponData->Damage, HitResult, ShotDirection, nullptr);
			HitActor->TakeDamage(WeaponData->Damage, DamageEvent, PlayerController, this);
		}
	}
}

void ATopDownCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveInput = Value.Get<FVector2D>();

	if (PlayerController != nullptr && MoveInput.SizeSquared() > 0.0f)
	{
		FVector MoveDirection = FVector(MoveInput, 0.0f);
		MoveDirection.Normalize();
		AddMovementInput(MoveDirection, 1.0f);
	}
}

void ATopDownCharacter::LookAt(FVector Target)
{
	FVector Direction = Target - this->GetActorLocation();
	Direction.Z = this->GetActorLocation().Z;
	FRotator LookAtRotation = UKismetMathLibrary::MakeRotFromX(Direction);
	LookAtRotation.Pitch = 0.0f;
	LookAtRotation.Roll = 0.0f;
	PlayerController->SetControlRotation(LookAtRotation);
}

void ATopDownCharacter::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
