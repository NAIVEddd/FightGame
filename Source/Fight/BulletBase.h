// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

UCLASS()
class FIGHT_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Bullet Motion")
		void SetVelocity(const FVector& velocity);

	UFUNCTION()
	void BulletHasCollide(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BulletComponent,
		meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BulletComponent,
		meta = (AllowPrivateAccess = "true"))
		class USphereComponent* BulletSphere;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BulletComponent,
		meta = (AllowPrivateAccess = "true"))
		class UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BulletMoveComponent,
		meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;
};
