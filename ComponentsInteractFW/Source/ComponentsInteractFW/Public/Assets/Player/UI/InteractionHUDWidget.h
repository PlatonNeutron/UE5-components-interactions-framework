// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class COMPONENTSINTERACTFW_API UInteractionHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateInteractionDisplay(const FText& NewText);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InteractionText;
};
