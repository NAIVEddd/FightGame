// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "WeaponBase.generated.h"

UCLASS()
class FIGHT_API AWeaponBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	TSubclassOf<class UUserWidget> PickupWidgetClass;
	UUserWidget* PickupWidget;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteracted_Implementation(const ASoldier* Player) override;
	virtual void OnInteractionRangeEntered_Implementation(const ASoldier* Player) override;
	virtual void OnInteractionRangeExited_Implementation() override;
};
