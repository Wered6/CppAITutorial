// Fill out your copyright notice in the Description page of Project Settings.


#include "CppAICharacterBase.h"
#include "Const.h"
#include "CppAITutorialCharacter.h"
#include "HealthBarWidget.h"
#include "NPC.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACppAICharacterBase::ACppAICharacterBase() :
	WidgetComponent{CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))},
	Health{MaxHealth},
	RightFistCollisionBox{CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollisionBox"))}
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region Nullchecks
	if (!WidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|WidgetComponent is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetRelativeLocation(defs::HealthBarZ);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{TEXT("/Game/UI/WBP_HealthBar")};
	if (WidgetClass.Succeeded())
	{
		WidgetComponent->SetWidgetClass(WidgetClass.Class);
	}

	RightFistCollisionBox->SetBoxExtent(defs::RightFistBoxSize, false);
	RightFistCollisionBox->SetupAttachment(GetMesh(), defs::RightFistSocketName);
	RightFistCollisionBox->SetRelativeLocation(defs::CollisionBoxLocation);
}

// Called when the game starts or when spawned
void ACppAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	RightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACppAICharacterBase::OnAttackOverlapBegin);
	RightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACppAICharacterBase::OnAttackOverlapEnd);
}

// Called every frame
void ACppAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#pragma region Nullchecks
	if (!WidgetComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|WidgetComponent is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	const UHealthBarWidget* const Widget{Cast<UHealthBarWidget>(WidgetComponent->GetUserWidgetObject())};

#pragma region Nullchecks
	if (!Widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|Widget is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	Widget->SetBarValuePercent(Health / MaxHealth);
}

// Called to bind functionality to input
void ACppAICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACppAICharacterBase::GetHealth() const
{
	return Health;
}

float ACppAICharacterBase::GetMaxHealth() const
{
	return MaxHealth;
}

void ACppAICharacterBase::SetHealth(float const NewHealth)
{
	Health = NewHealth;

	if (Health <= 0)
	{
		if (Cast<ACppAITutorialCharacter>(this))
		{
			UE_LOG(LogTemp, Error, TEXT("You Lose!!!!"))
		}
		else if (Cast<ANPC>(this))
		{
			UE_LOG(LogTemp, Error, TEXT("You Win!!!!"))
		}
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}

void ACppAICharacterBase::AttackStart() const
{
#pragma region Nullchecks
	if (!RightFistCollisionBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|RightFistCollisionBox is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void ACppAICharacterBase::AttackEnd() const
{
#pragma region Nullchecks
	if (!RightFistCollisionBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|RightFistCollisionBox is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	RightFistCollisionBox->SetCollisionProfileName("Fist");
	RightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}

void ACppAICharacterBase::OnAttackOverlapBegin(UPrimitiveComponent* const OverlappedComponent,
                                               AActor* const OtherActor,
                                               UPrimitiveComponent* const OtherComponent,
                                               int const OtherBodyIndex,
                                               bool const FromSweep,
                                               FHitResult const& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	ANPC* Enemy{Cast<ANPC>(OtherActor)};
	ACppAITutorialCharacter* Player{Cast<ACppAITutorialCharacter>(OtherActor)};
	if (Enemy)
	{
		float const NewHealth{Enemy->GetHealth() - Enemy->GetMaxHealth() * 0.1f};
		Enemy->SetHealth(NewHealth);
	}
	else if (Player)
	{
		float const NewHealth{Player->GetHealth() - Player->GetMaxHealth() * 0.07f};
		Player->SetHealth(NewHealth);
	}
}

void ACppAICharacterBase::OnAttackOverlapEnd(UPrimitiveComponent* const OverlappedComponent,
                                             AActor* const OtherActor,
                                             UPrimitiveComponent* OtherComponent,
                                             int const OtherBodyIndex)
{
}
