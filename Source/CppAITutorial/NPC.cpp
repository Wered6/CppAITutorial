// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
}

void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBehaviorTree* ANPC::GetBehaviorTree() const
{
	return Tree;
}

APatrolPath* ANPC::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* ANPC::GetMontage() const
{
	return Montage;
}

int ANPC::MeleeAttack_Implementation()
{
#pragma region NullChecks
	if (!Montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPC::MeleeAttack_Implementation|Montage is nullptr"))
		return 0;
	}
#pragma endregion

	PlayAnimMontage(Montage);

	return 0;
}
