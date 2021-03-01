// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Soldier.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthBar::SetOwner(ASoldier* o)
{
	Owner = o;
}

void UHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!Owner.IsValid())
		return;

	HealthBar->SetPercent(Owner->GetHealth() / Owner->GetMaxHealth());
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);
	CurrentHealthLabel->SetText(FText::AsNumber(Owner->GetHealth(), &Opts));
	MaxHealthLabel->SetText(FText::AsNumber(Owner->GetMaxHealth(), &Opts));
}
