#include "level_start.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BananoCharacter.h"

ALevelStartVolume::ALevelStartVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelStartVolume::OnOverlapBegin);

    // Este es el nivel al que quieres ir
    LevelToLoad = TEXT("nivel1");
}

void ALevelStartVolume::BeginPlay()
{
    Super::BeginPlay();
}

void ALevelStartVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ABananoCharacter* Character = Cast<ABananoCharacter>(OtherActor);
    if (Character && LevelToLoad != NAME_None)
    {
        UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
    }
}
