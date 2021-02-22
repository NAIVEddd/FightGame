// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class FIGHT_API ASoldier : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
		meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera,
		meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;
public:
	// Sets default values for this character's properties
	ASoldier();

	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
		void MoveForward(float value);
	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
		void MoveRight(float value);
	UFUNCTION(BlueprintCallable, Category = "PlayerActions")
		void ChangeDirection(float value);

	UFUNCTION(BlueprintCallable, Category = "Utilities")
		float CalculateDirection(const FVector& Velocity, const FRotator& Rotation);
	UFUNCTION(BlueprintCallable, Category = "Utilities")
		float CalculateSpeed(const FVector& Velocity, const FRotator& Rotation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate = 45.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
