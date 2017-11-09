// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pew.generated.h"

UCLASS()
class SPACESHOOTER_API APew : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APew();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComp;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
		class USphereComponent* CollisionComp;

	//Objeto responsável pela Partícula
	UPROPERTY(EditAnywhere)
		class UParticleSystemComponent* Particle;

};
