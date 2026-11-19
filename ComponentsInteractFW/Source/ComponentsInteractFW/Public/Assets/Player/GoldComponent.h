// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoldComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnGoldChanged,
	int32,
	NewGold
);

UCLASS(ClassGroup=(InteractionFW), meta=(BlueprintSpawnableComponent))
class COMPONENTSINTERACTFW_API UGoldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGoldComponent();

	UPROPERTY(BlueprintAssignable, Category="Gold")
	FOnGoldChanged OnGoldChanged;

	UFUNCTION(BlueprintCallable, Category="Gold")
	void AddGold(int32 Amount);

	UFUNCTION(BlueprintPure, Category="Gold")
	int32 GetGold() const;

private:
	UPROPERTY(VisibleAnywhere, Category="Gold")
	int32 Gold = 0;
};
