// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GoldHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class COMPONENTSINTERACTFW_API UGoldHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateGoldDisplay(int32 NewGold);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;
};
