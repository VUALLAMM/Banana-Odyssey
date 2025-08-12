#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimSequence.h"
#include "BananoAnimInstance.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EBananoAnimState : uint8
{
    Idle,
    Caminar,
    Dash,
    Dano,
    Salto,
    SaltoCaida,
    Caida,
    Resbaloso
};

UCLASS()
class BANANA_ODYSSEY_API UBananoAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UBananoAnimInstance();

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable)
        void CambiarEstadoAnim(EBananoAnimState NuevoEstado);

protected:
    UPROPERTY()
        UAnimSequence* AnimIdle;

    UPROPERTY()
        UAnimSequence* AnimCaminar;

    UPROPERTY()
        UAnimSequence* AnimSalto;

    UPROPERTY()
        UAnimSequence* AnimSaltoCaida;

    UPROPERTY()
        UAnimSequence* AnimCaida;

    UPROPERTY()
        UAnimSequence* AnimDash;

    UPROPERTY()
        UAnimSequence* AnimDano;

    UPROPERTY()
        UAnimSequence* AnimResbaloso;

    EBananoAnimState EstadoActual;

    // Blend time for transitions (tweak if needed)
    float BlendTime = 0.2f;

private:
    void PlayAnimation(UAnimSequence* AnimationToPlay, bool bLoop = false);

    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    // Current sequence being used by PlaySlotAnimationAsDynamicMontage
    UAnimSequence* AnimacionActual;

    // Previous state (to return after one-shot montages)
    EBananoAnimState EstadoPrevio;

    UPROPERTY()
        UAnimMontage* DashMontage;

    UPROPERTY()
        UAnimMontage* DanoMontage;

    UPROPERTY()
        UAnimMontage* SaltoMontage;

    UPROPERTY()
        UAnimMontage* SaltoCaidaMontage;

    UAnimMontage* CrearMontajeDesdeAnim(UAnimSequence* Anim);
};
