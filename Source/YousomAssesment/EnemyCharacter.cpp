#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "TopDownCharacter.h"
#include "Engine/DamageEvents.h"
#include "TopDownGameMode.h"
#include <NiagaraFunctionLibrary.h>

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	DamageTriggerRange = CreateDefaultSubobject<USphereComponent>(TEXT("DamageTriggerRange"));
	DamageTriggerRange->SetupAttachment(RootComponent);
	DamageTriggerRange->SetSphereRadius(50.0f);

	DamageTriggerRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::BeginOverlap);
	DamageTriggerRange->OnComponentEndOverlap.AddDynamic(this, &AEnemyCharacter::EndOverlap);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float RetVal = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHealth = CurrentHealth - RetVal;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFX, GetActorLocation());


	if (CurrentHealth <= 0)
	{
		ATopDownGameMode* GameMode = Cast<ATopDownGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->PawnKilled(this);
		Destroy();
	}

	return RetVal;
}

bool AEnemyCharacter::IsAlive() const
{
	return CurrentHealth>0;
}

void AEnemyCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATopDownCharacter* Player = Cast<ATopDownCharacter>(OtherActor))
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(DamageTimerHandle))
		{
			return;
		}
		ApplyDamage(Player);
		FTimerDelegate DamageDelegate;
		DamageDelegate.BindUFunction(this, FName("ApplyDamage"), Player);
		GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, DamageDelegate, DamageRate, true);
	}
}

void AEnemyCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
}

void AEnemyCharacter::ApplyDamage(AActor* TargetActor)
{
	TargetActor->TakeDamage(Damage, FDamageEvent(), GetController(), this);
}
