#include "MainMenuGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void AMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
        if (MenuWidget)
        {
            MenuWidget->AddToViewport();

            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->bShowMouseCursor = true;

                // Solo input para UI (no para juego)
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}
