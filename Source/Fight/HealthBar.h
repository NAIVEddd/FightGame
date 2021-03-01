// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FIGHT_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetOwner(class ASoldier* o);
private:
	TWeakObjectPtr<class ASoldier> Owner;
protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHealthLabel;
};
