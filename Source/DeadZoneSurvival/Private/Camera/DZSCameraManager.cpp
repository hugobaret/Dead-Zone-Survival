#include "Camera/DZSCameraManager.h"

#include "Player/DZSCharacter.h"
#include "Components/CapsuleComponent.h"

ADeadZoneSurvivalCameraManager::ADeadZoneSurvivalCameraManager()
{
	CrouchBlendTime = 0.0f;
	ViewPitchMin = -40.0f;
	ViewPitchMax = 30.0f;
}

void ADeadZoneSurvivalCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ADeadZoneSurvivalCharacter* DZSCharacter = Cast<ADeadZoneSurvivalCharacter>(
		GetOwningPlayerController()->GetPawn()))
	{
		UDeadZoneSurvivalCm* DZSMovementComponent = DZSCharacter->GetDeadZoneSurvivalMovementComponent();
		FVector TargetCrouchOffset = FVector(
			0.0f, 0.0f,
			DZSMovementComponent->GetCrouchedHalfHeight() - DZSCharacter->GetClass()->GetDefaultObject<ACharacter>()->
			                                                              GetCapsuleComponent()->
			                                                              GetScaledCapsuleHalfHeight());

		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset,
		                             FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.0f, 1.0f));

		if (DZSMovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.0f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.0f, CrouchBlendDuration);

		if (DZSMovementComponent->IsMovingOnGround())
			OutVT.POV.Location += Offset;
	}
}
