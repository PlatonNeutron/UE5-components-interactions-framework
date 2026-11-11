// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyInteractableComponent.generated.h"

// Déclaration des delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnEnterFeedback,
	APawn*,
	Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnExitFeedback,
	APawn*,
	Instigator
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnInteract,
	APawn*,
	Instigator
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMPONENTSINTERACTFW_API UMyInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInteractableComponent();
	
	// Events
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnEnterFeedback OnEnterFeedback;
	
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnExitFeedback OnExitFeedback;

	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteract OnInteract;
	
	// Editable Properties
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Interaction")
	bool bInteractionEnabled = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Interaction")
	FText InteractionText = FText::FromString(TEXT("Interact"));
	
	// Functions
	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetInteractionState(bool bState);
	
private:
	TWeakObjectPtr<APawn> NearbyPawn;
};
