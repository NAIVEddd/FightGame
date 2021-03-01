// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "WeaponBase.h"
#include "HealthBar.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASoldier::ASoldier()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// Set mesh infomation
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FQuat(FRotator(0.f, 270.f, 0.f)));

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 60.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(false);
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create a camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm
	CameraComponent->SetRelativeRotation(FRotator(15.f, 0.f, 0.f));

	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	Health = 100.f;
	MaxHealth = 100.f;
}

void ASoldier::MoveForward(float value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, value);
}

void ASoldier::MoveRight(float value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, value);
}

void ASoldier::ChangeDirection(float value)
{
	const float yaw = value * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator rotator = FRotator(0.f, yaw + Rotation.Yaw, 0.f);
	Controller->SetControlRotation(rotator);
	//SetActorRotation(rotator);
	AddActorLocalRotation(FQuat(FRotator(0.f, yaw, 0.f)));
}

void ASoldier::ActionInteract()
{
	TArray<AActor*> OverlapActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlapActors, TSubclassOf<AWeaponBase>(AWeaponBase::StaticClass()));
	for (AActor* weapon : OverlapActors)
	{
		IInteractionInterface* Interaction = Cast<IInteractionInterface>(weapon);
		Interaction->Execute_OnInteracted(weapon, this);
	}
}

float ASoldier::CalculateDirection(const FVector& Velocity, const FRotator& Rotation)
{
	const FVector right = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
	float angleToRight = FVector::DotProduct(Velocity.GetSafeNormal(), right);
	{
		//FString message = TEXT("Direction is ") + FString::SanitizeFloat(angleToRight);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, message);
	}
	return angleToRight;
}

float ASoldier::CalculateSpeed(const FVector& Velocity, const FRotator& Rotation)
{
	const FVector direction = Rotation.Vector();
	const float angleToForward = FVector::DotProduct(Velocity, direction);
	const float speed = Velocity.Size();
	if (angleToForward >= 0.f)
	{
		return speed;
	}
	return -speed;
}

void ASoldier::PickUpWeapon(AWeaponBase* weapon)
{
	if (IsValid(PrimaryWeapon))
	{
		PrimaryWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//PrimaryWeapon->DropWeapon();
	}
	PrimaryWeapon = weapon;
	PrimaryWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponHoldSocket"));
}

void ASoldier::Fire()
{
	if (IsFireable)
	{
		// do something.
		if (IsValid(PrimaryWeapon))
		{
			IsFire = true;
			PrimaryWeapon->Shoot(this);
		}
	}
}

void ASoldier::StopFire()
{
	if (IsFire && IsValid(PrimaryWeapon))
	{
		IsFire = false;
		PrimaryWeapon->StopShoot();
	}
}

void ASoldier::ReloadWeaponClip(int32 BulletNum)
{
	StopFire();
	SetFirestatus(false);
}

void ASoldier::ReloadWeaponClipDone()
{
	SetFirestatus(true);
}

void ASoldier::SetFirestatus(bool Fireable)
{
	IsFireable = Fireable;
	if (!Fireable && IsFire)
	{
		StopFire();
	}
}

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
{
	Super::BeginPlay();
	
	UHealthBar* healthbar = Cast<UHealthBar>(HealthWidgetComp->GetUserWidgetObject());
	if (IsValid(healthbar))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("get HealthBar success."));
		healthbar->SetOwner(this);
	}
}

// Called every frame
void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &ASoldier::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ASoldier::MoveRight);
	InputComponent->BindAxis("Turn", this, &ASoldier::ChangeDirection);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ASoldier::ActionInteract);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ASoldier::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &ASoldier::StopFire);
}

void ASoldier::GetDamage(float damage)
{
	Health -= damage;
	if (Health < 0.f)
	{
		IsDead = true;
		GetCapsuleComponent()->Deactivate();
		GetCharacterMovement()->DisableMovement();
	}
}
