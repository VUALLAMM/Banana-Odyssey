// MainMenuWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;

UCLASS()
class BANANA_ODYSSEY_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual bool Initialize() override;

    UPROPERTY(meta = (BindWidget))
        UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
        UButton* ExitButton;

    UFUNCTION()
        void OnPlayClicked();

    UFUNCTION()
        void OnExitClicked();
};
