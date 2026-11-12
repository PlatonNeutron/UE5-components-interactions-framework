// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInteractableComponent.h"
#include "GameFramework/Pawn.h"

// Sets default values for this component's properties
UMyInteractableComponent::UMyInteractableComponent()
{
	// 
	PrimaryComponentTick.bCanEverTick = false;
}

// Managing the state of the component
void UMyInteractableComponent::SetComponentState(bool bState)
{
	// Check if the component is in the same state as the wanted state
	if (bInteractionEnabled == bState)
	{
		return;
	}

	// Change it otherwise
	bInteractionEnabled = bState;
	
	// And call OnExitFeedback if the component turn false while a player is in the interaction zone
	if (!bInteractionEnabled && NearbyPawn.IsValid())
	{
		OnExitFeedback.Broadcast(NearbyPawn.Get());
	}
}

// 
// Notify all subscribers that a player entered in an interaction zone
void UMyInteractableComponent::NotifyPawnEntered(APawn* Pawn)
{
	// Check if the interaction is possible
	if (!IsValid(Pawn) || !bInteractionEnabled)
	{
		return;
	}
	
	// Check if the interaction doesn't have already been processed
	if (NearbyPawn.Get() == Pawn)
	{
		return;
	}

	NearbyPawn = Pawn;
	OnEnterFeedback.Broadcast(Pawn);
}

// Notify all subscribers that a player exited an interaction zone
void UMyInteractableComponent::NotifyPawnExited(APawn* Pawn)
{
	if (!NearbyPawn.IsValid() || NearbyPawn.Get() != Pawn)
	{
		return;
	}

	NearbyPawn.Reset();

	OnExitFeedback.Broadcast(Pawn);
}
