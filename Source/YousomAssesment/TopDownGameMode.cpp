#include "TopDownGameMode.h"
#include "TopDownCharacter.h"
#include "HUDWidget.h"
#include "EnemyCharacter.h"
#include "Engine/LevelStreaming.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "GameOverScreen.h"
#include <Kismet/GameplayStatics.h>
#include "TopDownPlayerController.h"

ATopDownGameMode::ATopDownGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/BP_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = ATopDownPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<ATopDownPlayerController> PlayerControllerBPClass(TEXT("/Game/Player/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void ATopDownGameMode::BeginPlay()
{
	UGameplayStatics::LoadStreamLevel(this, SubLevels[LevelIndex], true, true, FLatentActionInfo());
}

void ATopDownGameMode::PawnKilled(APawn* KilledPawn)
{
	if (ATopDownCharacter* Player = Cast<ATopDownCharacter>(KilledPawn))
	{
		GameOver();
	}
	else
	{
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(KilledPawn))
		{
			EnemyCount--;
			if (EnemyCount == 0)
			{
				EndWave();
			}
			if (PlayerHUD)
			{
				PlayerHUD->SetEnemyText(EnemyCount);
			}
		}
	}
}

void ATopDownGameMode::SetPlayerHUD(UHUDWidget* HUD)
{
	PlayerHUD = HUD;
	StartWave();
}

void ATopDownGameMode::SpawnEnemy()
{
	if (EnemyClass)
	{
		FVector SpawnLocation = FMath::RandPointInBox(FBox(FVector(-2000, -2000, 0), FVector(2000, 2000, 0)));
		FRotator SpawnRotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Enemy)
		{
			EnemyCount++;
		}
	}
}

void ATopDownGameMode::StartWave()
{
	WaveCount++;
	int EnemiesToSpawn = FMath::RandRange(MinEnemiesPerWave, MaxEnemiesPerWave);
	while (EnemyCount < EnemiesToSpawn)
	{
		SpawnEnemy();
	}
	if (PlayerHUD)
	{
		PlayerHUD->ShowWaveText(false);
		PlayerHUD->SetEnemyText(EnemyCount);
		PlayerHUD->ShowEnemyText(true);
	}
}

void ATopDownGameMode::EndWave()
{
	if (WaveCount < WavesPerLevel)
	{
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ATopDownGameMode::StartWave, TimeBetweenWaves, false);
		if (PlayerHUD)
		{
			PlayerHUD->SetWaveText(WaveCount);
			PlayerHUD->ShowWaveText(true);
			PlayerHUD->ShowEnemyText(false);
		}
	}
	else
	{
		WaveCount = 0;
		GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ATopDownGameMode::NextLevel, TimeBetweenWaves, false);
	}
}

void ATopDownGameMode::LoadLevel()
{
	LevelIndex++;
	if (LevelIndex >= SubLevels.Num())
	{
		LevelIndex = 0;
	}
	UGameplayStatics::LoadStreamLevel(this, SubLevels[LevelIndex], true, true, FLatentActionInfo());
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ATopDownGameMode::StartWave, TimeBetweenWaves, false);

}


void ATopDownGameMode::NextLevel()
{
	UGameplayStatics::UnloadStreamLevel(this, SubLevels[LevelIndex], FLatentActionInfo(), true);
	GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &ATopDownGameMode::LoadLevel, TimeBetweenWaves, false);

}

void ATopDownGameMode::GameOver()
{
	GameOverScreen = CreateWidget<UGameOverScreen>(GetWorld(), GameOverScreenClass);
	if (GameOverScreen)
	{
		GameOverScreen->AddToViewport();
	}
	UGameplayStatics::UnloadStreamLevel(this, SubLevels[LevelIndex], FLatentActionInfo(), true);
	GetWorld()->GetTimerManager().SetTimer(GameOverTimerHandle, this, &ATopDownGameMode::RestartGame, GameOverDelay, false);
}

void ATopDownGameMode::RestartGame()
{	
	GameOverScreen->RemoveFromParent();
	UGameplayStatics::OpenLevel(this, FName("StartMap_Persistent"), true);
}

