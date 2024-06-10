#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DeadZoneSurvivalCM.generated.h"


UCLASS()
class DEADZONESURVIVAL_API UDeadZoneSurvivalCm : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FDeadZoneSurvival_SaveMove final : public FSavedMove_Character
	{
	public:
		FDeadZoneSurvival_SaveMove();

	private:
		typedef FSavedMove_Character Super;

		uint8 bSavedIsSprinting : 1;

		virtual bool
		CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel,
		                        FNetworkPredictionData_Client_Character& ClientData) override;

		virtual void PrepMoveFor(ACharacter* Character) override;
	};

	class FDeadZoneSurvival_NetworkPredictionData final : public FNetworkPredictionData_Client_Character
	{
	public:
		typedef FNetworkPredictionData_Client_Character Super;

		explicit FDeadZoneSurvival_NetworkPredictionData(const UCharacterMovementComponent& ClientMovement);
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UPROPERTY(EditDefaultsOnly)
	float RunSpeed;
	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed;

public:
	UDeadZoneSurvivalCm();

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable, Category = "Character Movement: Sprint")
	void SprintPressed();

	UFUNCTION(BlueprintCallable, Category = "Character Movement: Sprint")
	void SprintReleased();

	bool bSafeIsSprinting = false;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
};
