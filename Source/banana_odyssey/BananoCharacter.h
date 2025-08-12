#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DashComponent.h"
#include "BananoCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UBoxComponent;

UCLASS()
class BANANA_ODYSSEY_API ABananoCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABananoCharacter();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void MoveRight(float Value);
    void StartJump();
    void StopJump();

    void HandleDash();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash", meta = (AllowPrivateAccess = "true"))
        UDashComponent* DashComponent;

    UFUNCTION()
        void OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable, Category = "Health")
        void ApplyCustomDamage(int DamageAmount);

    void ResetInvulnerability();
    void Die();

    // --- Momentum System ---
    void StartSpeedDecay();
    void ApplySpeedDecay();

public:
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
        float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
        bool bIsInAir;

    UPROPERTY(BlueprintReadOnly, Category = "Dash")
        bool bIsDashing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        int MaxLives = 3;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
        int CurrentLives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float InvulnerabilityTime = 1.5f;

    UPROPERTY(BlueprintReadOnly, Category = "Health")
        bool bIsInvulnerable;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
        UCameraComponent* SideViewCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
        UBoxComponent* Hitbox;

private:
    FTimerHandle InvulnerabilityTimer;

    // --- Dash control ---
    FTimerHandle DashCooldownTimer;
    float DashStrength = 1200.f;
    float DashCooldown = 1.f;
    bool bCanDash = true;

    // --- Momentum control ---
    float SpeedMultiplier = 1.0f;
    float SpeedStep = 0.25f;
    float MinMultiplier = 1.0f;
    float MaxMultiplier = 2.0f;

    FTimerHandle DecayStartTimer;
    FTimerHandle DecayTimer;
};
