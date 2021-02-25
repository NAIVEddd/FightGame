// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "WeaponStateEnum.h"
#include "WeaponBase.generated.h"

UCLASS()
class FIGHT_API AWeaponBase : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintCallable, Category = "Weapon State")
		void SetWeaponState(WeaponState state);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Property")
		int32 ClipCapacity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon Property")
		int32 CurrentClipBullets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Property")
		float RateOfFire;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	TSubclassOf<class UUserWidget> PickupWidgetClass;
	UUserWidget* PickupWidget;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// IInteractionInterface implement
	virtual void OnInteracted_Implementation(class ASoldier* Player) override;
	virtual void OnInteractionRangeEntered_Implementation(class ASoldier* Player) override;
	virtual void OnInteractionRangeExited_Implementation() override;

	// UFUNCTION(BlueprintCallable, Category = "WeaponActions")
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponActions")
		void Shoot(const class ASoldier* player);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponActions")
		void StopShoot();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "WeaponActions")
		int32 ReloadClip(int32 BulletNum);

	virtual void Shoot_Implementation(const class ASoldier* player);
	virtual void StopShoot_Implementation();
	virtual int32 ReloadClip_Implementation(int32 BulletNum);

	// delegate OnOutOfClip
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponComp,
		meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = WeaponComp,
		meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComp;

	WeaponState m_WeaponState;

	// How much time left to shoot next bullet, Controled by RateOfFire
	float ShootInterval;
	float ShootTime = 0.f;

	const class ASoldier* m_Player;

	// interactive ui
	TSubclassOf<class UUserWidget> SimpleUIClass;
	class UUserWidget* SimpleUIComponent;

	TSubclassOf<class AActor> BulletClass;
};
