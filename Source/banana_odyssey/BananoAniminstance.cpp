#include "BananoAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"

UBananoAnimInstance::UBananoAnimInstance()
    : AnimIdle(nullptr)
    , AnimCaminar(nullptr)
    , AnimSalto(nullptr)
    , AnimSaltoCaida(nullptr)
    , AnimCaida(nullptr)
    , AnimDash(nullptr)
    , AnimDano(nullptr)
    , AnimResbaloso(nullptr)
    , AnimacionActual(nullptr)
    , DashMontage(nullptr)
    , DanoMontage(nullptr)
    , SaltoMontage(nullptr)
    , SaltoCaidaMontage(nullptr)
    , EstadoActual(EBananoAnimState::Idle)
{
    // Cargar assets desde constructor (es lo correcto)
    static ConstructorHelpers::FObjectFinder<UAnimSequence> IdleObj(TEXT("/Game/animaciones/banano_idle.banano_idle"));
    if (IdleObj.Succeeded()) AnimIdle = IdleObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_idle"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> CaminarObj(TEXT("/Game/animaciones/banano_caminar.banano_caminar"));
    if (CaminarObj.Succeeded()) AnimCaminar = CaminarObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_caminar"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> SaltoObj(TEXT("/Game/animaciones/banano_salto.banano_salto"));
    if (SaltoObj.Succeeded()) AnimSalto = SaltoObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_salto"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> SaltoCaidaObj(TEXT("/Game/animaciones/banano_saltocaida.banano_saltocaida"));
    if (SaltoCaidaObj.Succeeded()) AnimSaltoCaida = SaltoCaidaObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_saltocaida"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> CaidaObj(TEXT("/Game/animaciones/banano_caida.banano_caida"));
    if (CaidaObj.Succeeded()) AnimCaida = CaidaObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_caida"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> DashObj(TEXT("/Game/animaciones/banano_ataque.banano_ataque"));
    if (DashObj.Succeeded()) AnimDash = DashObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_ataque"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> DanoObj(TEXT("/Game/animaciones/banano_dano.banano_dano"));
    if (DanoObj.Succeeded()) AnimDano = DanoObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_dano"));

    static ConstructorHelpers::FObjectFinder<UAnimSequence> ResbalosoObj(TEXT("/Game/animaciones/banano_resbaloso.banano_resbaloso"));
    if (ResbalosoObj.Succeeded()) AnimResbaloso = ResbalosoObj.Object;
    else UE_LOG(LogTemp, Warning, TEXT("BananoAnimInstance: could not find banano_resbaloso"));
}

void UBananoAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // Crear montajes dinámicos (one-shot)
    DashMontage = CrearMontajeDesdeAnim(AnimDash);
    DanoMontage = CrearMontajeDesdeAnim(AnimDano);
    SaltoMontage = CrearMontajeDesdeAnim(AnimSalto);
    SaltoCaidaMontage = CrearMontajeDesdeAnim(AnimSaltoCaida);

    EstadoActual = EBananoAnimState::Idle;
    AnimacionActual = nullptr;

    // Bind un delegate global para el fin de montajes en esta instancia
    FOnMontageEnded MontageEndedDelegate;
    MontageEndedDelegate.BindUObject(this, &UBananoAnimInstance::OnMontageEnded);
    Montage_SetEndDelegate(MontageEndedDelegate);
}

void UBananoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!AnimIdle) return;

    // Si un montage one-shot está reproduciéndose, no forzamos cambio de secuencia
    if ((DashMontage && Montage_IsPlaying(DashMontage)) ||
        (DanoMontage && Montage_IsPlaying(DanoMontage)) ||
        (SaltoMontage && Montage_IsPlaying(SaltoMontage)) ||
        (SaltoCaidaMontage && Montage_IsPlaying(SaltoCaidaMontage)))
    {
        return;
    }

    switch (EstadoActual)
    {
    case EBananoAnimState::Idle:
        if (AnimacionActual != AnimIdle)
            PlayAnimation(AnimIdle, true);
        break;
    case EBananoAnimState::Caminar:
        if (AnimacionActual != AnimCaminar)
            PlayAnimation(AnimCaminar, true);
        break;
    case EBananoAnimState::Caida:
        if (AnimacionActual != AnimCaida)
            PlayAnimation(AnimCaida, true);
        break;
    case EBananoAnimState::Resbaloso:
        if (AnimacionActual != AnimResbaloso)
            PlayAnimation(AnimResbaloso, true);
        break;
    default:
        // Los estados one-shot (Dash, Dano, Salto, SaltoCaida) se manejan con montajes
        break;
    }
}

void UBananoAnimInstance::CambiarEstadoAnim(EBananoAnimState NuevoEstado)
{
    // Si el montaje de daño está sonando, tiene prioridad
    if (DanoMontage && Montage_IsPlaying(DanoMontage) && NuevoEstado != EBananoAnimState::Dano)
        return;

    // Si un one-shot está sonando, limitamos cambios
    if ((DashMontage && Montage_IsPlaying(DashMontage)) ||
        (SaltoMontage && Montage_IsPlaying(SaltoMontage)) ||
        (SaltoCaidaMontage && Montage_IsPlaying(SaltoCaidaMontage)))
    {
        if (NuevoEstado != EBananoAnimState::Dash &&
            NuevoEstado != EBananoAnimState::Salto &&
            NuevoEstado != EBananoAnimState::SaltoCaida)
        {
            return;
        }
    }

    if (NuevoEstado == EstadoActual)
        return;

    EstadoPrevio = EstadoActual;
    EstadoActual = NuevoEstado;

    switch (NuevoEstado)
    {
    case EBananoAnimState::Dash:
        if (DashMontage) Montage_Play(DashMontage, 1.f);
        break;
    case EBananoAnimState::Dano:
        if (DanoMontage) Montage_Play(DanoMontage, 1.f);
        break;
    case EBananoAnimState::Salto:
        if (SaltoMontage) Montage_Play(SaltoMontage, 1.f);
        break;
    case EBananoAnimState::SaltoCaida:
        if (SaltoCaidaMontage) Montage_Play(SaltoCaidaMontage, 1.f);
        break;
    default:
        // Otros estados se procesan en NativeUpdateAnimation
        break;
    }
}

void UBananoAnimInstance::PlayAnimation(UAnimSequence* AnimationToPlay, bool bLoop)
{
    if (!AnimationToPlay) return;
    if (AnimacionActual == AnimationToPlay) return;

    AnimacionActual = AnimationToPlay;

    // Mezcla y detiene montajes actuales
    Montage_Stop(BlendTime);

    // Reproducir la sequence como dynamic montage en DefaultSlot
    PlaySlotAnimationAsDynamicMontage(
        AnimationToPlay,
        FName("DefaultSlot"),
        BlendTime,
        BlendTime,
        1.f,
        bLoop ? 0 : 1
    );
}

void UBananoAnimInstance::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Si uno de nuestros montajes terminó, volver al estado previo
    if (Montage == DanoMontage || Montage == DashMontage || Montage == SaltoMontage || Montage == SaltoCaidaMontage)
    {
        EstadoActual = EstadoPrevio;
        AnimacionActual = nullptr; // forzar reevaluación en NativeUpdateAnimation
    }
}

UAnimMontage* UBananoAnimInstance::CrearMontajeDesdeAnim(UAnimSequence* Anim)
{
    if (!Anim) return nullptr;

    UAnimMontage* NewMontage = NewObject<UAnimMontage>(this, NAME_None, RF_Transient);
    if (!NewMontage) return nullptr;

    NewMontage->SetSkeleton(Anim->GetSkeleton());

    FCompositeSection Section;
    Section.SectionName = FName("DefaultSection");
    NewMontage->CompositeSections.Add(Section);

    FSlotAnimationTrack SlotTrack;
    SlotTrack.SlotName = FName("DefaultSlot");

    FAnimSegment Segment;
    Segment.AnimReference = Anim;
    Segment.AnimStartTime = 0.f;
    Segment.AnimEndTime = Anim->GetPlayLength();
    Segment.AnimPlayRate = 1.f;
    Segment.LoopingCount = 1;

    SlotTrack.AnimTrack.AnimSegments.Add(Segment);
    NewMontage->SlotAnimTracks.Add(SlotTrack);

    NewMontage->SequenceLength = Anim->GetPlayLength();

    return NewMontage;
}
