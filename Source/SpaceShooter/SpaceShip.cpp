// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceShip.h"
#include "Pew.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"


// Sets default values
ASpaceShip::ASpaceShip()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>
		(TEXT("Camera"));
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	Camera->OrthoWidth = 2048.0f;
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Camera->SetWorldLocation(FVector(0.0f, 0.0f, 500.0f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(
		TEXT("StaticMesh'/Game/Assets/Meshes/SM_Pixel_Player.SM_Pixel_Player'"));

	if (Mesh.Succeeded()) {
		MeshComp->SetStaticMesh(Mesh.Object);
	}
	MeshComp->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));

	Particle = CreateDefaultSubobject<UParticleSystemComponent>
		(TEXT("ParticleSystem"));
	Particle->bAutoActivate = false;
	ConstructorHelpers::FObjectFinder<UParticleSystem> LoadParticle
	(TEXT("ParticleSystem'/Game/Assets/Particles/P_Pixel_Explosion_Player.P_Pixel_Explosion_Player'"));
	if (LoadParticle.Succeeded()) {
		Particle->SetTemplate(LoadParticle.Object);
	}
	Particle->SetupAttachment(RootComponent);

	Life = 5;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(Shoot, this,
		&ASpaceShip::PewPew, 0.20f, true);
	GetWorldTimerManager().PauseTimer(Shoot);
	
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Camera->SetWorldLocation(FVector(0.0f, 0.0f, 500.0f));
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveX", this, &ASpaceShip::MoveX);
	InputComponent->BindAxis("MoveY", this, &ASpaceShip::MoveY);

	PlayerInputComponent->BindAction("PewPew", IE_Pressed, this, &ASpaceShip::ShootPew);
	PlayerInputComponent->BindAction("PewPew", IE_Released, this, &ASpaceShip::StopShoot);


}

void ASpaceShip::SetCamera(UCameraComponent * NewCamera)
{
	Camera = NewCamera;
}

int ASpaceShip::GetLife()
{
	return Life;
}

void ASpaceShip::SetLife(int NewLife)
{
	Life = NewLife;
}

void ASpaceShip::MoveX(float Delta)
{
	if (Delta > 0) {
		MeshComp->SetRelativeRotation(FRotator(MeshComp->GetComponentRotation().Pitch, 90.0f, -110.0));
	}
	
	else if (Delta < 0) {
		MeshComp->SetRelativeRotation(FRotator(MeshComp->GetComponentRotation().Pitch, 90.0f, -70.0));
	}
	FVector Side(1.0f, 0.0f, 0.0f);//X - Y - Z
	AddMovementInput(Side, Delta);
	if (GetActorLocation().X >= 450) {
		SetActorLocation(FVector(450, GetActorLocation().Y, GetActorLocation().Z));
	} else if (GetActorLocation().X <= -450) {
		SetActorLocation(FVector(-450, GetActorLocation().Y, GetActorLocation().Z));
	}
}

void ASpaceShip::MoveY(float Delta)
{
	if (Delta > 0) {
		MeshComp->SetRelativeRotation(FRotator(-20.0f, 90.0f, MeshComp->GetComponentRotation().Roll));
	}
	else if (Delta < 0) {
		MeshComp->SetRelativeRotation(FRotator(20.0f, 90.0f, MeshComp->GetComponentRotation().Roll));
	}
	FVector Side(0.0f, 1.0f, 0.0f);//X - Y - Z
	AddMovementInput(Side, Delta);
	if (GetActorLocation().Y >= 1000) {
		SetActorLocation(FVector(GetActorLocation().X, 1000, GetActorLocation().Z));
	}
	else if (GetActorLocation().Y <= -1000) {
		SetActorLocation(FVector(GetActorLocation().X, -1000, GetActorLocation().Z));
	}
}

void ASpaceShip::PewPew()
{
	UWorld* World = GetWorld();
	if (World) {
		FVector Location = MeshComp->GetComponentLocation();
		FActorSpawnParameters SpawnParameters;
		APew* Pew =
			World->SpawnActor<APew>(PewBP, Location,
				FRotator::ZeroRotator, SpawnParameters);
		UE_LOG(LogTemp, Warning, TEXT("Comando"));
		//Pew->SetIndex(1);
	}
	
}

void ASpaceShip::DestroyShip() {
	MeshComp->SetVisibility(false);
	Particle->ToggleActive();
	GetWorldTimerManager().SetTimer(TimerDestroy, this,
		&ASpaceShip::TimeToDestroy, 1.0f, false);

}

void ASpaceShip::ShootPew()
{
	GetWorldTimerManager().UnPauseTimer(Shoot);
}

void ASpaceShip::StopShoot() {
	GetWorldTimerManager().PauseTimer(Shoot);
}

void ASpaceShip::TimeToDestroy()
{
	GetWorldTimerManager().ClearTimer(TimerDestroy);
	Destroy();
}

