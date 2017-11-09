// Fill out your copyright notice in the Description page of Project Settings.

#include "MyManger.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "SpaceShip.h"
#include "Enemy.h"


// Sets default values
AMyManger::AMyManger()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Dificulty = 10000;
	/*
	Camera = CreateDefaultSubobject<UCameraComponent>
		(TEXT("Camera"));
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	Camera->OrthoWidth = 2048.0f;
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Camera->SetWorldLocation(FVector(0.0f, 0.0f, 500.0f));

	ConstructorHelpers::FObjectFinder<UClass> PewPewBP(
		TEXT("Blueprint'/Game/Blueprints/SpaceShipBP.SpaceShipBP_C'"));

	
	if (PewPewBP.Succeeded()) {
		//Seta o TSubclassOf.
		PewPewShip = Cast<UClass>(PewPewBP.Object);
	}
	*/
}

// Called when the game starts or when spawned
void AMyManger::BeginPlay()
{
	Super::BeginPlay();
	

}

// Called every frame
void AMyManger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	int MyRand = FMath::RandRange(0,Dificulty);
	if (MyRand >= 9900) {
		SpawnEnemy();
	}


}

// Called to bind functionality to input
void AMyManger::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyManger::SpawnEnemy()
{
	UWorld* World = GetWorld();
	if (World) {
		if (PewPewShip) {
			float MyY = FMath::RandRange(-1000, 1000);
			FActorSpawnParameters SpawnParameters;
			AEnemy* Enemy = World->SpawnActor<AEnemy>(PewPewShip, FVector(550.0f, MyY, 110.0f),
				FRotator::ZeroRotator, SpawnParameters);
			Dificulty= Dificulty + 1;

		}
	}
}

