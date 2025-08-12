#include "FallDamage.h"
#include "Components/BoxComponent.h"
#include "BananoCharacter.h"
#include "Kismet/GameplayStatics.h"

AFallDamage::AFallDamage()
{
    PrimaryActorTick.bCanEverTick = false;

    Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
    RootComponent = Hitbox;

    // Configurar para generar overlap solo con Pawn
    Hitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Hitbox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    Hitbox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Hitbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AFallDamage::OnOverlapBegin);

    DamageAmount = 10.f;
    ImpulseStrength = 2000.f;
}

void AFallDamage::BeginPlay()
{
    Super::BeginPlay();
}

void AFallDamage::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ABananoCharacter* Character = Cast<ABananoCharacter>(OtherActor);
    if (Character)
    {
        // Aplica daño
        UGameplayStatics::ApplyDamage(Character, DamageAmount, nullptr, this, nullptr);

        // Impulsa hacia arriba con la fuerza dada
        FVector LaunchVelocity(0.f, 0.f, ImpulseStrength);
        Character->LaunchCharacter(LaunchVelocity, true, true);
    }
}
