// Fill out your copyright notice in the Description page of Project Settings.

#include "MyInteractableComponent.h"

// Sets default values for this component's properties
UMyInteractableComponent::UMyInteractableComponent()
{
	// 
	PrimaryComponentTick.bCanEverTick = false;
}

// Managing the state of the component
void UMyInteractableComponent::SetInteractionState(bool bState)
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
