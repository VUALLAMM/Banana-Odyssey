// LevelTransitionVolume.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

class UBoxComponent;

UCLASS()
class BANANA_ODYSSEY_API ALevelTransitionVolume : public AActor
{
    GENERATED_BODY()

public:
    ALevelTransitionVolume();

protected:
    UPROPERTY(VisibleAnywhere)
        UBoxComponent* CollisionBox;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere)
        FName LevelToLoad;

    virtual void BeginPlay() override;
};
