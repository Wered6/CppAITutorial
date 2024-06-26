// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatInterface.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

class APatrolPath;
class UBehaviorTree;

UCLASS()
class CPPAITUTORIAL_API ANPC : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	ANPC();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

#pragma region Input

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion

#pragma region AI

public:
	UBehaviorTree* GetBehaviorTree() const;

	APatrolPath* GetPatrolPath() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBehaviorTree> Tree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI", meta=(AllowPrivateAccess="true"))
	TObjectPtr<APatrolPath> PatrolPath;

#pragma endregion

#pragma region Animation

public:
	UAnimMontage* GetMontage() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAnimMontage> Montage;

#pragma endregion

#pragma region Combat

public:
	virtual int MeleeAttack_Implementation() override;

#pragma endregion
};
