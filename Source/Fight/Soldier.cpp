// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
{
	Super::BeginPlay();
	
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
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
}
