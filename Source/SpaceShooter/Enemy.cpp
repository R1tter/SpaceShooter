// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "SpaceShip.h"
#include "Pew.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject
		<UCapsuleComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);

	CollisionComp->SetCapsuleRadius(34.0f);
	CollisionComp->SetCapsuleHalfHeight(88.0f);
	RootComponent = CollisionComp;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(
		TEXT("StaticMesh'/Game/Assets/Meshes/SM_Pixel_Enemy_1.SM_Pixel_Enemy_1'"));

	if (Mesh.Succeeded()) {
		MeshComp->SetStaticMesh(Mesh.Object);
	}
	MeshComp->SetRelativeRotation(FRotator(0.0f, -90.0f, -90.0f));


	Particle = CreateDefaultSubobject<UParticleSystemComponent>
		(TEXT("ParticleSystem"));
	Particle->bAutoActivate = false;
	ConstructorHelpers::FObjectFinder<UParticleSystem> LoadParticle
	(TEXT("ParticleSystem'/Game/Assets/Particles/P_Pixel_Explosion_Enemy.P_Pixel_Explosion_Enemy'"));
	if (LoadParticle.Succeeded()) {
		Particle->SetTemplate(LoadParticle.Object);
	}
	Particle->SetupAttachment(RootComponent);

	Life = 2;


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();

}

int AEnemy::GetLife()
{
	return Life;
}

void AEnemy::SetLife(int NewLife)
{
	Life = NewLife;
}

bool AEnemy::IsAlive()
{
	return MeshComp->IsVisible();
}

void AEnemy::DestroyEnemy()
{
	MeshComp->SetVisibility(false);
	Particle->ToggleActive();
	GetWorldTimerManager().SetTimer(TimerDestroy, this,
		&AEnemy::TimeToDestroy, 1.0f, false);
	//Destroy();
}

void AEnemy::TimeToDestroy() {
	GetWorldTimerManager().ClearTimer(TimerDestroy);
	Destroy();
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != nullptr && OtherActor->IsA(ASpaceShip::StaticClass())) {
		ASpaceShip* Ship = Cast<ASpaceShip>(OtherActor);
		if (MeshComp->IsVisible()) {
			Ship->SetLife(Ship->GetLife() - 1);
			if (Ship->GetLife() <= 0) {
				Ship->DestroyShip();
			}
		}
		DestroyEnemy();
	}
}

void AEnemy::Move() {
	SetActorLocation(FVector(GetActorLocation().X - 1, GetActorLocation().Y, GetActorLocation().Z));
	if (GetActorLocation().X <= -550) {
		Destroy();
	}

}