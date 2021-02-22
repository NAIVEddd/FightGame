// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Soldier.h"
#include "MyUserWidgetBase.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnInteracted_Implementation(const ASoldier* Player)
{
}

void AWeaponBase::OnInteractionRangeEntered_Implementation(const ASoldier* Player)
{
	APlayerController* playerCtrl = Cast<APlayerController>(Player->GetController());
	
}

void AWeaponBase::OnInteractionRangeExited_Implementation()
{
}

