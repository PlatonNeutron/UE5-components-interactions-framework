// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class APawn;
class USphereComponent;

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
	
	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	void UpdateProximitySphere();
};
