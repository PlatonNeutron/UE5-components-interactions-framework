// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractorComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MyInteractableComponent.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

// Sets default values for this component's properties
UInteractorComponent::UInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// PAWN
// Manage when the player controller switch pawn
void UInteractorComponent::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	ClearNearbyInteractables();
	CurrentPawn = NewPawn;
	UpdateProximitySphere();
}


// PROXIMITY SPHERE
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


// HANDLER
//
void UInteractorComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if overlap should be processed
	if (!IsValid(OtherActor) || !CurrentPawn.IsValid())
	{
		return;
	}

	// Get the UMyInteractableComponent of the actor if interactable
	UMyInteractableComponent* Interactable = OtherActor->FindComponentByClass<UMyInteractableComponent>();

	if (!IsValid(Interactable))
	{
		return;
	}
	
	CleanupInvalidInteractables();
	
	// Add the interactable in the list
	NearbyInteractables.Add(Interactable);
	
	// Delegate to listen if the actor gets destroyed
	OtherActor->OnDestroyed.AddUniqueDynamic(this, &UInteractorComponent::HandleInteractableDestroyed);

	// Delegate to enable feedbacks
	Interactable->NotifyPawnEntered(CurrentPawn.Get());
	
	//UE_LOG(LogTemp, Warning, TEXT("Interactables proches : %d"), NearbyInteractables.Num());
}

//
void UInteractorComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if overlap should be processed
	if (!IsValid(OtherActor) || !CurrentPawn.IsValid())
	{
		return;
	}

	// Get the UMyInteractableComponent of the actor if interactable
	UMyInteractableComponent* Interactable = OtherActor->FindComponentByClass<UMyInteractableComponent>();

	if (!IsValid(Interactable))
	{
		return;
	}

	// Gestion of overlapping on the same actor, no suppression if still overlapping
	if (ProximitySphere.IsValid() && ProximitySphere->IsOverlappingActor(OtherActor))
	{
		return;
	}
	
	if (NearbyInteractables.Remove(Interactable) == 0)
	{
		return;
	}

	// Remove the delegate to listen if the actor gets destroyed
	OtherActor->OnDestroyed.RemoveDynamic(this, &UInteractorComponent::HandleInteractableDestroyed);
	
	// Delegate to enable feedbacks
	Interactable->NotifyPawnExited(CurrentPawn.Get());
	
	//UE_LOG(LogTemp, Warning, TEXT("Interactables proches : %d"), NearbyInteractables.Num());
}

// Handler to clear any destroyed actor
void UInteractorComponent::HandleInteractableDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor)
	{
		return;
	}

	for (auto It = NearbyInteractables.CreateIterator(); It; ++It)
	{
		UMyInteractableComponent* Interactable = It->Get();

		if (!IsValid(Interactable) || Interactable->GetOwner() == DestroyedActor)
		{
			It.RemoveCurrent();
		}
	}
}


//
// Fait avec IA plus le temps et trop fatigué pour y arriver seul
void UInteractorComponent::ClearNearbyInteractables()
{
	for (const TWeakObjectPtr<UMyInteractableComponent>& InteractableIterator : NearbyInteractables)
	{
		UMyInteractableComponent* Interactable = InteractableIterator.Get();

		if (!IsValid(Interactable))
		{
			continue;
		}

		AActor* Owner = Interactable->GetOwner();

		// Remove delegate for each 
		if (IsValid(Owner))
		{
			Owner->OnDestroyed.RemoveDynamic(this, &UInteractorComponent::HandleInteractableDestroyed);
		}

		if (CurrentPawn.IsValid())
		{
			Interactable->NotifyPawnExited(CurrentPawn.Get());
		}
	}

	NearbyInteractables.Empty();
}

//
void UInteractorComponent::CleanupInvalidInteractables()
{
	for (auto It = NearbyInteractables.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}


//
bool UInteractorComponent::FindLookedAtInteractable(UMyInteractableComponent*& OutInteractable) const
{
	OutInteractable = nullptr;

	if (!CurrentPawn.IsValid())
	{
		return false;
	}

	APlayerController* PC = Cast<APlayerController>(GetOwner());

	if (!IsValid(PC))
	{
		return false;
	}

	FVector CameraLocation;
	FRotator CameraRotation;

	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector TraceStart = CameraLocation;

	const FVector TraceEnd =
		TraceStart + CameraRotation.Vector() * 500.0f;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CurrentPawn.Get());

	FHitResult HitResult;

	const bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(15.0f),
		QueryParams
	);

	if (!bHit || !IsValid(HitResult.GetActor()))
	{
		return false;
	}

	UMyInteractableComponent* Interactable =
		HitResult.GetActor()
		->FindComponentByClass<UMyInteractableComponent>();

	if (!IsValid(Interactable))
	{
		return false;
	}

	if (!Interactable->bInteractionEnabled)
	{
		return false;
	}

	if (!NearbyInteractables.Contains(Interactable))
	{
		return false;
	}

	OutInteractable = Interactable;

	return true;
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
	
	ClearNearbyInteractables();
	
	CurrentPawn.Reset();
	UpdateProximitySphere();
	
	Super::EndPlay(EndPlayReason);
}
