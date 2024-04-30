#include "TopDownPlayerController.h"
#include "TopDownCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/KismetSystemLibrary.h>

ATopDownPlayerController::ATopDownPlayerController()
{
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}
