// Fill out your copyright notice in the Description page of Project Settings.

#include "Pew.h"
#include "SpaceShip.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
APew::APew()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject
		<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APew::OnOverlapBegin);
	CollisionComp->SetSphereRadius(10.0f);
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName("NoCollision");
	ConstructorHelpers::FObjectFinder<UStaticMesh> LoadMesh(TEXT("StaticMesh'/Game/Assets/Meshes/SM_Pixel_Cube.SM_Pixel_Cube'"));
	if (LoadMesh.Succeeded()) {
		MeshComp->SetStaticMesh(LoadMesh.Object);
	}
	MeshComp->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject
		<UProjectileMovementComponent>(TEXT("Projectile"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 800.0f;
	ProjectileMovement->MaxSpeed = 800.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>
		(TEXT("ParticleSystem"));
	//Desabilita a auto ativação da partículo ao criar
	Particle->bAutoActivate = false;
	//Carrega a partícula a ser utilizada
	ConstructorHelpers::FObjectFinder<UParticleSystem> LoadParticle
	(TEXT("ParticleSystem'/Game/Assets/Particles/P_Pixel_Engine.P_Pixel_Engine'"));
	//Verifica se conseguiu carregar com sucesso
	if (LoadParticle.Succeeded()) {
		//Define o templete do componente referente a partícula
		//como a partícula carregada.
		Particle->SetTemplate(LoadParticle.Object);
	}
	//Anexa a partícula ao Root Component
	Particle->SetupAttachment(RootComponent);

	InitialLifeSpan = 4.0f;
}

// Called when the game starts or when spawned
void APew::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Spawn"));
	Particle->ToggleActive();
}

// Called every frame
void APew::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APew::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != nullptr) {
		if (!OtherActor->IsA(ASpaceShip::StaticClass()) && OtherActor->IsA(AEnemy::StaticClass())) {
			AEnemy* Enemy = Cast<AEnemy>(OtherActor);
			if (Enemy->IsAlive()) {
				Enemy->SetLife(Enemy->GetLife() - 1);
				if (Enemy->GetLife() <= 0) {
					Enemy->DestroyEnemy();
				}
				Destroy();
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("HIT!"));
	}
}
