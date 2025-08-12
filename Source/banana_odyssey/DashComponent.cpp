#include "DashComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UDashComponent::UDashComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    CurrentSpeedMultiplier = 1.0f;
    MaxSpeedMultiplier = 2.0f;
    SpeedReductionDelay = 1.25f;
    TimeSinceLastDash = 0.0f;

    bIsDashing = false;
    bDashOnCooldown = false;
}

void UDashComponent::BeginPlay()
{
    Super::BeginPlay();
    OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!OwnerCharacter) return;

    if (!bIsDashing && !bDashOnCooldown && CurrentSpeedMultiplier > 1.0f)
    {
        TimeSinceLastDash += DeltaTime;

        if (TimeSinceLastDash >= SpeedReductionDelay)
        {
            CurrentSpeedMultiplier = FMath::Max(1.0f, CurrentSpeedMultiplier - 0.25f * DeltaTime * 2);
            OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f * CurrentSpeedMultiplier;
        }
    }
}

void UDashComponent::Dash()
{
    if (bDashOnCooldown || !OwnerCharacter) return;

    bIsDashing = true;
    bDashOnCooldown = true;
    TimeSinceLastDash = 0.0f;

    CurrentSpeedMultiplier = FMath::Min(MaxSpeedMultiplier, CurrentSpeedMultiplier + 0.25f);
    OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f * CurrentSpeedMultiplier;

    FVector Direction = OwnerCharacter->GetActorForwardVector();
    OwnerCharacter->LaunchCharacter(Direction * 500.0f, true, true);

    GetWorld()->GetTimerManager().SetTimer(DashEndTimer, this, &UDashComponent::StopDash, 1.0f, false);
    GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, this, &UDashComponent::StartDashCooldown, 1.0f, false);
}

void UDashComponent::StopDash()
{
    bIsDashing = false;
}

void UDashComponent::StartDashCooldown()
{
    bDashOnCooldown = false;
}

