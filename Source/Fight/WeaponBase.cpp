// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Soldier.h"
#include "BulletBase.h"
#include "MyUserWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include <Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget>
		UserWidgetBPClass(TEXT("/Game/UI/SimpleUI"));
	if (UserWidgetBPClass.Class != nullptr)
	{
		SimpleUIClass = UserWidgetBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor>
		BulletBPClass(TEXT("/Game/Items/Gun/Bullet/BP_Bullet"));
	if (BulletBPClass.Class != nullptr)
	{
		BulletClass = BulletBPClass.Class;
	}

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SphereComp->InitSphereRadius(120.f);
	SphereComp->SetupAttachment(SkeletalMeshComp);

	ClipCapacity = 30;
	CurrentClipBullets = ClipCapacity;
	RateOfFire = 3.f;
}

void AWeaponBase::SetWeaponState(WeaponState state)
{
	switch (state)
	{
	case WEAPON_Droped:
	{
		SphereComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		SkeletalMeshComp->SetCollisionProfileName(TEXT("DroppedWeapon"));
		break;
	}
	case WEAPON_Equiped:
	{
		SphereComp->SetCollisionProfileName(TEXT("NoCollision"), false);
		SkeletalMeshComp->SetCollisionProfileName(TEXT("NoCollision"), false);
		break;
	}
	}
	m_WeaponState = state;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	ShootInterval = 1.f / RateOfFire;
	ShootTime = 0;

	SetWeaponState(WeaponState::WEAPON_Droped);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	do
	{
		if (IsValid(m_Player))
		{
			ShootTime += DeltaTime;
			if (ShootTime < ShootInterval) break;
			ShootTime = 0.f;

			FVector location = m_Player->GetActorLocation();
			FVector forward = m_Player->GetActorForwardVector();
			FVector gunheadLocation = location;	// location from gun mesh
			FVector endLocation = forward * 1000.f + location;
			FVector bulletVelocity = (endLocation - gunheadLocation).GetSafeNormal() * 10000.f;

			ABulletBase* bullet = Cast<ABulletBase>(GetWorld()->SpawnActor(BulletClass.Get(), &gunheadLocation));
			bullet->SetVelocity(bulletVelocity);
		}
	} while (false);
}

void AWeaponBase::OnInteracted_Implementation(class ASoldier* Player)
{
	Player->PickUpWeapon(this);
	SetWeaponState(WeaponState::WEAPON_Equiped);
}

void AWeaponBase::OnInteractionRangeEntered_Implementation(class ASoldier* Player)
{
	APlayerController* playerCtrl = Cast<APlayerController>(Player->GetController());
	if (!IsValid(SimpleUIComponent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Press F to PickUP Weapon."));
		//impleUIComponent = CreateWidget<UUserWidget>(GetWorld(), SimpleUIClass);
		
		//SimpleUIComponent->SetWidgetSpace(EWidgetSpace::Screen);
		//SimpleUIComponent->SetVisibility(ESlateVisibility::Visible);
		//SimpleUIComponent->SetDrawAtDesiredSize(true);
		//SimpleUIComponent->RegisterComponent();
	}
}

void AWeaponBase::OnInteractionRangeExited_Implementation()
{
}

void AWeaponBase::Shoot_Implementation(const class ASoldier* player)
{
	m_Player = player;
	ShootTime = ShootInterval - 0.01;
}

void AWeaponBase::StopShoot_Implementation()
{
	m_Player = nullptr;
}

int32 AWeaponBase::ReloadClip_Implementation(int32 BulletNum)
{
	return int32();
}

