// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractorComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// PAWN
// Manage when the player controller switch pawn
void UInteractorComponent::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	CurrentPawn = NewPawn;
}


// SPHERE
void UInteractorComponent::UpdateProximitySphere()
{
	ProximitySphere.Reset();

	if (!CurrentPawn.IsValid())
	{
		return;
	}

	TArray<USphereComponent*> Spheres;
	CurrentPawn->GetComponents<USphereComponent>(Spheres);

	for (USphereComponent* Sphere : Spheres)
	{
		if (IsValid(Sphere) && Sphere->ComponentHasTag(TEXT("InteractionProximity")))
		{
			ProximitySphere = Sphere;
			return;
		}
	}
}


// UNREAL
void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the PlayerController to which this component is attached
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (!IsValid(PlayerController))
	{
		return;
	}
	
	// Subscribing to OnPossessedPawnChanged, to get notified of a pawn switch 
	PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UInteractorComponent::HandlePossessedPawnChanged);
	
	// Initializing the pawn
	CurrentPawn = PlayerController->GetPawn();
}

void UInteractorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Get the PlayerController to which this component is attached
	APlayerController* PlayerController = Cast<APlayerController>(GetOwner());

	if (IsValid(PlayerController))
	{
		// Unsub of OnPossessedPawnChanged
		PlayerController->OnPossessedPawnChanged.RemoveDynamic(this, &UInteractorComponent::HandlePossessedPawnChanged);
	}

	CurrentPawn.Reset();
	Super::EndPlay(EndPlayReason);
}
