#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "level_end.generated.h"  // ESTE debe ir aquí, al final de los includes

UCLASS()
class BANANA_ODYSSEY_API ALevelEnd : public AActor
{
    GENERATED_BODY()

public:
    ALevelEnd();

protected:
    UPROPERTY(VisibleAnywhere)
        UBoxComponent* CollisionBox;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void BeginPlay() override;
};
