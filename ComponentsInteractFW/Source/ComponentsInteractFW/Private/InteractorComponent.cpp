// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractorComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MyInteractableComponent.h"

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
	UpdateProximitySphere();
}


// SPHERE
void UInteractorComponent::UpdateProximitySphere()
{
	UnbindProximitySphere();
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
			BindProximitySphere();
			return;
		}
	}
}

//
void UInteractorComponent::BindProximitySphere()
{
	if (!ProximitySphere.IsValid())
	{
		return;
	}

	ProximitySphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &UInteractorComponent::HandleBeginOverlap);
	ProximitySphere->OnComponentEndOverlap.AddUniqueDynamic(this, &UInteractorComponent::HandleEndOverlap);
}

//
void UInteractorComponent::UnbindProximitySphere()
{
	if (!ProximitySphere.IsValid())
	{
		return;
	}

	ProximitySphere->OnComponentBeginOverlap.RemoveDynamic(this, &UInteractorComponent::HandleBeginOverlap);
	ProximitySphere->OnComponentEndOverlap.RemoveDynamic(this, &UInteractorComponent::HandleEndOverlap);
}


//
void UInteractorComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	UMyInteractableComponent* Interactable =
		OtherActor->FindComponentByClass<UMyInteractableComponent>();

	if (!IsValid(Interactable))
	{
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Objet interactif detecte : %s"),
		*OtherActor->GetName()
	);
}

//
void UInteractorComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	UMyInteractableComponent* Interactable =
		OtherActor->FindComponentByClass<UMyInteractableComponent>();

	if (!IsValid(Interactable))
	{
		return;
	}

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("EndOverlap avec : %s"),
		*OtherActor->GetName()
	);
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
	// Initializing the Proximity Sphere
	UpdateProximitySphere();
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
	UpdateProximitySphere();
	
	Super::EndPlay(EndPlayReason);
}
