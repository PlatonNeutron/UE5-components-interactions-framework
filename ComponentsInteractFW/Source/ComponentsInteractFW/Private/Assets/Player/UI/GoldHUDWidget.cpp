// Fill out your copyright notice in the Description page of Project Settings.

#include "Assets/Player/UI/GoldHUDWidget.h"
#include "Components/TextBlock.h"

void UGoldHUDWidget::UpdateGoldDisplay(int32 NewGold)
{
	if (!IsValid(GoldText))
	{
		return;
	}

	GoldText->SetText(FText::Format(NSLOCTEXT("GoldHUD", "GoldFormat", "Gold : {0}"), FText::AsNumber(NewGold)));
}
