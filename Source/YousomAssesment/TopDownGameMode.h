#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownGameMode.generated.h"

class UGameOverScreen;
class AEnemyCharacter;

UCLASS()
class YOUSOMASSESMENT_API ATopDownGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATopDownGameMode();

	void BeginPlay() override;

	void PawnKilled(APawn* KilledPawn);

	void SetPlayerHUD(class UHUDWidget* HUD);

private:
	void SpawnEnemy();
	void StartWave();
	void EndWave();
	void NextLevel();
	void LoadLevel();
	void GameOver();
	void RestartGame();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditDefaultsOnly)
	float GameOverDelay = 2.0f;
	UPROPERTY(EditDefaultsOnly)
	int32 WavesPerLevel = 5;
	UPROPERTY(EditDefaultsOnly)
	float MinEnemiesPerWave = 5;
	UPROPERTY(EditDefaultsOnly)
	float MaxEnemiesPerWave = 10;
	UPROPERTY(EditDefaultsOnly)
	float TimeBetweenWaves = 1.0f;
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> SubLevels;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameOverScreen> GameOverScreenClass;
	UPROPERTY()
	UGameOverScreen* GameOverScreen;
	UPROPERTY()
	UHUDWidget* PlayerHUD;
	UPROPERTY(EditDefaultsOnly)
	FBox SpawnBox = FBox(FVector(-2000, -2000, 0), FVector(2000, 2000, 0));

	int32 WaveCount = 0;
	int32 EnemyCount = 0;
	int32 LevelIndex = 0;

	struct FTimerHandle WaveTimerHandle;
	struct FTimerHandle GameOverTimerHandle;
};
