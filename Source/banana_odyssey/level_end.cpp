#include "level_end.h"
#include "Kismet/GameplayStatics.h"
#include "BananoCharacter.h"

ALevelEnd::ALevelEnd()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelEnd::OnOverlapBegin);
}

void ALevelEnd::BeginPlay()
{
    Super::BeginPlay();
}

void ALevelEnd::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ABananoCharacter* Character = Cast<ABananoCharacter>(OtherActor);
    if (Character)
    {
        // Cierra el juego
        UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
    }
}
