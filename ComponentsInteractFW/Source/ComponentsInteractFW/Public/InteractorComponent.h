// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractorComponent.generated.h"

class APawn;

UCLASS( ClassGroup=(InteractionFW), meta=(BlueprintSpawnableComponent) )
class COMPONENTSINTERACTFW_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractorComponent();
	
private:
	TWeakObjectPtr<APawn> CurrentPawn;
	
	void UpdateCurrentPawn();
};
