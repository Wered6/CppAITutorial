// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ChangeSpeed.h"
#include "AIController.h"
#include "NPC.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	const AAIController* const AIC{OwnerComp.GetAIOwner()};

#pragma  region NullChecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_ChangeSpeed::OnBecomeRelevant|AIC is nullptr"))
		return;
	}
#pragma endregion

	const ANPC* const NPC{Cast<ANPC>(AIC->GetPawn())};

#pragma region NullChecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTService_ChangeSpeed::OnBecomeRelevant|NPC is nullptr"))
		return;
	}
#pragma endregion

	NPC->GetCharacterMovement()->MaxWalkSpeed = Speed;
}
