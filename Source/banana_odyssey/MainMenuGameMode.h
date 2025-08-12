#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class BANANA_ODYSSEY_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    // Aquí asignas tu Widget Blueprint en el editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
        TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
    UUserWidget* MenuWidget;
};
