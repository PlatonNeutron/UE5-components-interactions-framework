// Fill out your copyright notice in the Description page of Project Settings.

#include "Assets/Player/GoldComponent.h"

UGoldComponent::UGoldComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGoldComponent::AddGold(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	Gold += Amount;

	OnGoldChanged.Broadcast(Gold);

	UE_LOG(LogTemp, Warning, TEXT("Gold actuel : %d"), Gold);
}

int32 UGoldComponent::GetGold() const
{
	return Gold;
}