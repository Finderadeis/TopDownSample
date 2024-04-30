#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

UCLASS()
class YOUSOMASSESMENT_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* WaveText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* EnemyText;

	void SetHealthBarPercent(float Percent);

	void ShowWaveText(bool bShow);

	void SetWaveText(int32 Wave);

	void ShowEnemyText(bool bShow);

	void SetEnemyText(int32 EnemiesLeft);
};
