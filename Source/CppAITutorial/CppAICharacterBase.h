// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CppAICharacterBase.generated.h"

class UWidgetComponent;

UCLASS()
class CPPAITUTORIAL_API ACppAICharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACppAICharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Input
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region Health

public:
	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const NewHealth);

private:
	UPROPERTY()
	TObjectPtr<UWidgetComponent> WidgetComponent;
	float const MaxHealth{100.f};
	float Health;
	
#pragma endregion 
};
