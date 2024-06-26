// Fill out your copyright notice in the Description page of Project Settings.


#include "CppAICharacterBase.h"
#include "Const.h"
#include "HealthBarWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ACppAICharacterBase::ACppAICharacterBase() : WidgetComponent{
	                                             CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue"))
                                             }, Health{MaxHealth}
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
}

// Called when the game starts or when spawned
void ACppAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
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
}
