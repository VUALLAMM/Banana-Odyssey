#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SlideTrigger.generated.h"

UCLASS()
class BANANA_ODYSSEY_API ASlideTrigger : public AActor
{
    GENERATED_BODY()

public:
    ASlideTrigger();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
        UBoxComponent* TriggerBox;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
        void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
    class ABananoCharacter* OverlappingCharacter;

    // Dirección del impulso, 1 o -1 (por ejemplo, 1 = derecha)
    float ImpulseDirection;

    // Fuerza del impulso constante
    float ImpulseStrength = 1500.f;
};
