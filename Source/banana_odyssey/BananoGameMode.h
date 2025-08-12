#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BananoGameMode.generated.h"

UCLASS()
class BANANA_ODYSSEY_API ABananoGameMode : public AGameModeBase
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
