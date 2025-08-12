// MainMenuWidget.cpp
#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

bool UMainMenuWidget::Initialize()
{
    if (!Super::Initialize()) return false;

    if (PlayButton)
        PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);

    if (ExitButton)
        ExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitClicked);

    return true;
}

void UMainMenuWidget::OnPlayClicked()
{
    UGameplayStatics::OpenLevel(this, TEXT("nivel1")); // Cambia por tu nombre exacto de nivel 1
}

void UMainMenuWidget::OnExitClicked()
{
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
