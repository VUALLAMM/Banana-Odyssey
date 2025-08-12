#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "menu.generated.h" // Debe coincidir con el nombre del archivo .h

class UBoxComponent;

UCLASS()
class BANANA_ODYSSEY_API AMenuVolume : public AActor
{
    GENERATED_BODY()

public:
    AMenuVolume();

protected:
    UPROPERTY(VisibleAnywhere)
        UBoxComponent* CollisionBox;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere)
        FName LevelToLoad;

    virtual void BeginPlay() override;
};
