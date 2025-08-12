#include "BananoCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"  // <-- Añadido para OpenLevel

ABananoCharacter::ABananoCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

    GetCharacterMovement()->GravityScale = 2.0f;
    GetCharacterMovement()->AirControl = 0.8f;
    GetCharacterMovement()->JumpZVelocity = 1000.f;
    GetCharacterMovement()->GroundFriction = 3.0f;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bInheritYaw = false;
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    SideViewCamera->bUsePawnControlRotation = false;

    Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
    Hitbox->SetupAttachment(GetMesh());
    Hitbox->SetBoxExtent(FVector(40.f, 20.f, 40.f));
    Hitbox->SetCollisionProfileName(TEXT("Trigger"));

    DashComponent = CreateDefaultSubobject<UDashComponent>(TEXT("DashComponent"));

    Speed = 0.f;
    bIsInAir = false;
    bIsDashing = false;

    MaxLives = 3;
    CurrentLives = MaxLives;
    InvulnerabilityTime = 1.5f;
    bIsInvulnerable = false;
}

void ABananoCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (Hitbox)
    {
        Hitbox->OnComponentBeginOverlap.AddDynamic(this, &ABananoCharacter::OnHitboxOverlap);
    }
}

void ABananoCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Speed = GetVelocity().Size2D();
    bIsInAir = GetCharacterMovement()->IsFalling();

    // Aplicar la velocidad multiplicada
    GetCharacterMovement()->MaxWalkSpeed = 600.f * SpeedMultiplier;

    const FVector Velocity = GetVelocity();
    if (FMath::Abs(Velocity.X) > KINDA_SMALL_NUMBER)
    {
        const float Sign = FMath::Sign(Velocity.X);
        FRotator NewRot = GetActorRotation();
        NewRot.Yaw = (Sign > 0.f) ? 0.f : 180.f;
        SetActorRotation(NewRot);
    }

    if (CameraBoom)
    {
        CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
    }
}

void ABananoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveRight", this, &ABananoCharacter::MoveRight);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABananoCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABananoCharacter::StopJump);
    PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABananoCharacter::HandleDash);
}

void ABananoCharacter::HandleDash()
{
    if (!bCanDash) return;

    bIsDashing = true;
    bCanDash = false;

    // Incrementar velocidad acumulada
    SpeedMultiplier = FMath::Clamp(SpeedMultiplier + SpeedStep, MinMultiplier, MaxMultiplier);

    // Aplicar impulso horizontal manteniendo velocidad vertical
    FVector DashDirection = GetActorForwardVector();
    LaunchCharacter(FVector(DashDirection.X * DashStrength, 0.f, GetCharacterMovement()->Velocity.Z), true, true);

    // Reiniciar timers de decay
    GetWorld()->GetTimerManager().ClearTimer(DecayStartTimer);
    GetWorld()->GetTimerManager().ClearTimer(DecayTimer);

    // Esperar 1.5s antes de empezar a reducir velocidad
    GetWorld()->GetTimerManager().SetTimer(DecayStartTimer, this, &ABananoCharacter::StartSpeedDecay, 1.5f, false);

    // Cooldown del dash
    GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, [this]()
        {
            bIsDashing = false;
            bCanDash = true;
        }, DashCooldown, false);
}

void ABananoCharacter::StartSpeedDecay()
{
    // Reducir velocidad cada 0.5s
    GetWorld()->GetTimerManager().SetTimer(DecayTimer, this, &ABananoCharacter::ApplySpeedDecay, 0.5f, true);
}

void ABananoCharacter::ApplySpeedDecay()
{
    if (SpeedMultiplier > MinMultiplier)
    {
        SpeedMultiplier = FMath::Max(SpeedMultiplier - SpeedStep, MinMultiplier);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(DecayTimer);
    }
}

void ABananoCharacter::MoveRight(float Value)
{
    AddMovementInput(FVector(1.f, 0.f, 0.f), Value);
}

void ABananoCharacter::StartJump()
{
    Jump();
}

void ABananoCharacter::StopJump()
{
    StopJumping();
}

void ABananoCharacter::OnHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    if (OtherActor->ActorHasTag(TEXT("Danger")))
    {
        ApplyCustomDamage(1);
    }
}

void ABananoCharacter::ApplyCustomDamage(int DamageAmount)
{
    if (bIsInvulnerable || DamageAmount <= 0) return;

    CurrentLives -= DamageAmount;

    if (GEngine)
    {
        FString Msg = FString::Printf(TEXT("Daño recibido: -%d vida(s) | Vidas restantes: %d"), DamageAmount, CurrentLives);
        GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, Msg);
    }

    if (CurrentLives <= 0)
    {
        Die();
        return;
    }

    bIsInvulnerable = true;
    GetWorld()->GetTimerManager().SetTimer(InvulnerabilityTimer, this, &ABananoCharacter::ResetInvulnerability, InvulnerabilityTime, false);
}

void ABananoCharacter::ResetInvulnerability()
{
    bIsInvulnerable = false;
}

void ABananoCharacter::Die()
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (PC)
    {
        DisableInput(PC);
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Muerto"));
    }

    // Reiniciar nivel para hacer respawn
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));

    // Resetear vidas e invulnerabilidad para nuevo intento
    CurrentLives = MaxLives;
    bIsInvulnerable = false;
}
