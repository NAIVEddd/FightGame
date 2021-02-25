// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "Components/SphereComponent.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABulletBase::ABulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BulletSphere"));
	BulletSphere->InitSphereRadius(1.f);
	BulletSphere->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AddRelativeRotation(FRotator(0.f, -90.f, 0.f));
	StaticMesh->SetupAttachment(BulletSphere);

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetRelativeLocation(FVector(-3.5f, 0.f, 0.f));
	PointLight->CastShadows = 0;
	PointLight->SetIntensity(3000.f);
	PointLight->SetAttenuationRadius(100.f);
	PointLight->SetupAttachment(BulletSphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::SetVelocity(const FVector& velocity)
{
	ProjectileMovement->Velocity = velocity;
}

