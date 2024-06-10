#include "DeadZoneSurvival/Public/DeadZoneSurvivalCM.h"

#include "GameFramework/Character.h"

#pragma region UDeadZoneSurvivalCm Implementation

UDeadZoneSurvivalCm::UDeadZoneSurvivalCm()
{
	bOrientRotationToMovement = true;
	RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	JumpZVelocity = 700.f;
	AirControl = 0.35f;
	MaxWalkSpeed = 300.0f;
	MinAnalogWalkSpeed = 20.f;
	BrakingDecelerationWalking = 2000.f;
	BrakingDecelerationFalling = 1500.0f;

	WalkSpeed = 300.0f;
	RunSpeed = 600.0f;

	NavAgentProps.bCanCrouch = true;
}

FNetworkPredictionData_Client* UDeadZoneSurvivalCm::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UDeadZoneSurvivalCm* MutableThis = const_cast<UDeadZoneSurvivalCm*>(this);

		MutableThis->ClientPredictionData = new FDeadZoneSurvival_NetworkPredictionData(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

#pragma region Input

void UDeadZoneSurvivalCm::SprintPressed()
{
	bSafeIsSprinting = true;
}

void UDeadZoneSurvivalCm::SprintReleased()
{
	bSafeIsSprinting = false;
}

void UDeadZoneSurvivalCm::CrouchToggle()
{
	bWantsToCrouch = !bWantsToCrouch;
}

#pragma endregion

void UDeadZoneSurvivalCm::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	bSafeIsSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UDeadZoneSurvivalCm::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if (MovementMode == MOVE_Walking)
	{
		if (bSafeIsSprinting)
			MaxWalkSpeed = RunSpeed;
		else
			MaxWalkSpeed = WalkSpeed;
	}
}

#pragma endregion

#pragma region FDeadZoneSurvival_SaveMove Implementation

UDeadZoneSurvivalCm::FDeadZoneSurvival_SaveMove::FDeadZoneSurvival_SaveMove()
{
	bSavedIsSprinting = 0;
}

bool UDeadZoneSurvivalCm::FDeadZoneSurvival_SaveMove::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                     ACharacter* InCharacter, float MaxDelta) const
{
	FDeadZoneSurvival_SaveMove* NewDeadZoneSurvivalMove = static_cast<FDeadZoneSurvival_SaveMove*>(NewMove.Get());

	if (this->bSavedIsSprinting != NewDeadZoneSurvivalMove->bSavedIsSprinting)
		return false;

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UDeadZoneSurvivalCm::FDeadZoneSurvival_SaveMove::Clear()
{
	Super::Clear();
	bSavedIsSprinting = 0;
}


uint8 UDeadZoneSurvivalCm::FDeadZoneSurvival_SaveMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedIsSprinting)
		Result |= FLAG_Custom_0;

	return Result;
}

void UDeadZoneSurvivalCm::FDeadZoneSurvival_SaveMove::SetMoveFor(ACharacter* Character, float InDeltaTime,
                                                                 FVector const& NewAccel,
                                                                 FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UDeadZoneSurvivalCm* DeadZoneSurvivalCm = Cast<UDeadZoneSurvivalCm>(Character->GetCharacterMovement());

	bSavedIsSprinting = DeadZoneSurvivalCm->bSafeIsSprinting;
}

void UDeadZoneSurvivalCm::FDeadZoneSurvival_SaveMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UDeadZoneSurvivalCm* DeadZoneSurvivalCm = Cast<UDeadZoneSurvivalCm>(Character->GetCharacterMovement());

	DeadZoneSurvivalCm->bSafeIsSprinting = bSavedIsSprinting;
}

#pragma endregion

#pragma region FDeadZoneSurvival_NetworkPredictionData Implementation

UDeadZoneSurvivalCm::FDeadZoneSurvival_NetworkPredictionData::FDeadZoneSurvival_NetworkPredictionData(
	const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UDeadZoneSurvivalCm::FDeadZoneSurvival_NetworkPredictionData::AllocateNewMove()
{
	return MakeShared<FDeadZoneSurvival_SaveMove>();
}

#pragma endregion
