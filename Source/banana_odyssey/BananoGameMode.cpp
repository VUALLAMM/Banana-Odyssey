#include "BananoGameMode.h"
#include "GameFramework/PlayerController.h"

void ABananoGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->ConsoleCommand("viewmode unlit");
    }
}
