#include "menu.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BananoCharacter.h"

AMenuVolume::AMenuVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMenuVolume::OnOverlapBegin);

    // Cargar el nivel "menu"
    LevelToLoad = TEXT("menu");
}

void AMenuVolume::BeginPlay()
{
    Super::BeginPlay();
}

void AMenuVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    ABananoCharacter* Character = Cast<ABananoCharacter>(OtherActor);
    if (Character && LevelToLoad != NAME_None)
    {
        UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
    }
}
