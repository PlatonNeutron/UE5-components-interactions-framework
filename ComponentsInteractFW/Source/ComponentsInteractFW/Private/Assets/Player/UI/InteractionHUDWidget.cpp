// Fill out your copyright notice in the Description page of Project Settings.

#include "Assets/Player/UI/InteractionHUDWidget.h"
#include "Components/TextBlock.h"

void UInteractionHUDWidget::UpdateInteractionDisplay(const FText& NewText)
{
	if (!IsValid(InteractionText))
	{
		return;
	}

	InteractionText->SetText(NewText);
}
