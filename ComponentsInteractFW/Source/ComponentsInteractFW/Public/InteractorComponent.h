// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

// Anticipated declarations
class APawn;
class USphereComponent;
class UPrimitiveComponent;
class AActor;
class UMyInteractableComponent;

// 
UCLASS( ClassGroup=(InteractionFW), meta=(BlueprintSpawnableComponent) )
class COMPONENTSINTERACTFW_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:
	TWeakObjectPtr<APawn> CurrentPawn;
	TWeakObjectPtr<USphereComponent> ProximitySphere;
	TSet<TWeakObjectPtr<UMyInteractableComponent>> NearbyInteractables;
	TWeakObjectPtr<UMyInteractableComponent> FocusedInteractable;
	bool FindLookedAtInteractable(UMyInteractableComponent*& OutInteractable) const;
	
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void HandleInteractableDestroyed(AActor* DestroyedActor);
	
	void UpdateProximitySphere();
	void BindProximitySphere();
	void UnbindProximitySphere();
	void CleanupInvalidInteractables();
	void ClearNearbyInteractables();
};
