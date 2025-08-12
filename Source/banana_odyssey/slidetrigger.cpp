#include "SlideTrigger.h"
#include "Components/BoxComponent.h"
#include "BananoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ASlideTrigger::ASlideTrigger()
{
    PrimaryActorTick.bCanEverTick = true;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASlideTrigger::OnOverlapBegin);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASlideTrigger::OnOverlapEnd);

    OverlappingCharacter = nullptr;

    ImpulseStrength = 1500.f; // velocidad fija a la derecha
}

void ASlideTrigger::BeginPlay()
{
    Super::BeginPlay();
}

void ASlideTrigger::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OverlappingCharacter)
    {
        FVector CurrentVelocity = OverlappingCharacter->GetCharacterMovement()->Velocity;

        // Forzar solo velocidad X a 1500 (derecha)
        FVector NewVelocity = FVector(ImpulseStrength, CurrentVelocity.Y, CurrentVelocity.Z);

        OverlappingCharacter->GetCharacterMovement()->Velocity = NewVelocity;

        // Debug visual para asegurar que el trigger está activo
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("Personaje impulsado a la derecha"));
        }
    }
}

void ASlideTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        ABananoCharacter* Character = Cast<ABananoCharacter>(OtherActor);
        if (Character)
        {
            OverlappingCharacter = Character;

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Entró al trigger"));
            }
        }
    }
}

void ASlideTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor == OverlappingCharacter)
    {
        // Al salir dejar de forzar la velocidad
        OverlappingCharacter = nullptr;

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Salió del trigger"));
        }
    }
}
