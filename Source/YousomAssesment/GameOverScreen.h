#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverScreen.generated.h"

UCLASS()
class YOUSOMASSESMENT_API UGameOverScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* GameOverImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* GameOverText;
};
