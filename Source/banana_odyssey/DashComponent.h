#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BANANA_ODYSSEY_API UDashComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UDashComponent();

protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
        void Dash();

    UFUNCTION(BlueprintCallable)
        bool IsDashing() const { return bIsDashing; }

private:
    float CurrentSpeedMultiplier;
    float MaxSpeedMultiplier;
    float SpeedReductionDelay;
    float TimeSinceLastDash;
    bool bIsDashing;
    bool bDashOnCooldown;

    FTimerHandle DashEndTimer;
    FTimerHandle DashCooldownTimer;

    void StopDash();
    void StartDashCooldown();

    ACharacter* OwnerCharacter;
};
