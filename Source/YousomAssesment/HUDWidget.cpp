#include "HUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UHUDWidget::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UHUDWidget::ShowWaveText(bool bShow)
{
	if (WaveText)
	{
		WaveText->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetWaveText(int32 Wave)
{
	FText Text = FText::Format(FText::FromString("Wave {0} cleared"), FText::AsNumber(Wave));
	if (WaveText)
	{
		WaveText->SetText(Text);
	}
}

void UHUDWidget::ShowEnemyText(bool bShow)
{
	if (EnemyText)
	{
		EnemyText->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UHUDWidget::SetEnemyText(int32 EnemiesLeft)
{
	FText Text = FText::Format(FText::FromString("Enemies left: {0}"), FText::AsNumber(EnemiesLeft));
	if (EnemyText)
	{
		EnemyText->SetText(Text);
	}
}
