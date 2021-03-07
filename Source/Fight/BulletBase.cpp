// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBase.h"
#include "Soldier.h"
#include "GameFramework/Actor.h"
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
	//BulletSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
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
	BulletSphere->OnComponentBeginOverlap.AddDynamic(this, &ABulletBase::BulletHasCollide);
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABulletBase::SetVelocity(const FVector& velocity)
{
	if (IsValid(this))
	{
		ProjectileMovement->Velocity = velocity;
	}
}

void ABulletBase::BulletHasCollide(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ( OtherActor == this || OtherActor->ActorHasTag(TEXT("Player")))
	{
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Hit Enemy"));
	if (IsValid(OverlappedComp) && OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		ASoldier* soldier = Cast<ASoldier>(OtherActor);
		if(IsValid(soldier))
		{
			soldier->GetDamage(13.f);
		}
	}
	Destroy();
}

