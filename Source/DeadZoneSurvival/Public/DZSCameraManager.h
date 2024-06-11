#pragma once

#include "CoreMinimal.h"
#include "DZSCameraManager.generated.h"

UCLASS()
class DEADZONESURVIVAL_API ADeadZoneSurvivalCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = 0.5f;

	float CrouchBlendTime;

public:
	ADeadZoneSurvivalCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
};
