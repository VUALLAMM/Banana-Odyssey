#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallDamage.generated.h"

class UBoxComponent;

UCLASS()
class BANANA_ODYSSEY_API AFallDamage : public AActor
{
    GENERATED_BODY()

public:
    AFallDamage();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
        UBoxComponent* Hitbox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
        float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
        float ImpulseStrength;

    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
