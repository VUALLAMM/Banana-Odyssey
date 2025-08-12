#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Impulso.generated.h"

class UBoxComponent;

UCLASS()
class BANANA_ODYSSEY_API AImpulso : public AActor
{
    GENERATED_BODY()

public:
    AImpulso();

protected:
    virtual void BeginPlay() override;

    // Función que se llama cuando otro actor entra en el trigger
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
            const FHitResult& SweepResult);

public:
    // Caja de colisión
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impulso")
        UBoxComponent* Hitbox;

    // Fuerza vertical de impulso
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impulso")
        float ImpulseStrength;
};
