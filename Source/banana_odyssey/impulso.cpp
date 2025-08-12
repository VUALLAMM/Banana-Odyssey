#include "Impulso.h"
#include "Components/BoxComponent.h"
#include "BananoCharacter.h"

AImpulso::AImpulso()
{
    PrimaryActorTick.bCanEverTick = false;

    Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
    RootComponent = Hitbox;

    // Configuración de colisión: solo detectar al jugador
    Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Hitbox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    Hitbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Hitbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AImpulso::OnOverlapBegin);

    // Valor por defecto del impulso
    ImpulseStrength = 4000.f;
}

void AImpulso::BeginPlay()
{
    Super::BeginPlay();
}

void AImpulso::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ABananoCharacter* Character = Cast<ABananoCharacter>(OtherActor);
    if (Character)
    {
        // Mantener velocidad horizontal y solo modificar la vertical
        FVector CurrentVelocity = Character->GetVelocity();
        FVector LaunchVelocity(CurrentVelocity.X, CurrentVelocity.Y, ImpulseStrength);

        Character->LaunchCharacter(LaunchVelocity, true, true);
    }
}
