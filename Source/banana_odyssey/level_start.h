#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "level_start.generated.h"

class UBoxComponent;

UCLASS()
class BANANA_ODYSSEY_API ALevelStartVolume : public AActor
{
    GENERATED_BODY()

public:
    ALevelStartVolume();

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
