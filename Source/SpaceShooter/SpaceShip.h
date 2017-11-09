// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceShip.generated.h"

UCLASS()
class SPACESHOOTER_API ASpaceShip : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceShip();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetCamera(class UCameraComponent* NewCamera);

	int GetLife();

	void DestroyShip();

	void SetLife(int NewLife);

private:
	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class APew> PewBP;


	void MoveX(float Delta);

	void MoveY(float Delta);

	void PewPew();

	int Life;

	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Particle;

	

	FTimerHandle TimerDestroy;
	FTimerHandle Shoot;

	void ShootPew();

	void StopShoot();

	void TimeToDestroy();
};
