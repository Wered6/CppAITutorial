// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerInMeleeRange.h"
#include "NPC.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_IsPlayerInMeleeRange::UBTService_IsPlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player In Melee Range");
}

void UBTService_IsPlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get controller and player
	const ANPC_AIController* const NPCAIC{Cast<ANPC_AIController>(OwnerComp.GetAIOwner())};
	const AActor* const Player{UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)};

#pragma region Nullchecks
	if (!NPCAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|NPCAIC is nullptr"), *FString(__FUNCTION__))
		return;
	}
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|Player is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	// get NPC
	const ANPC* const NPC{Cast<ANPC>(NPCAIC->GetPawn())};

#pragma region Nullchecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|NPC is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	// write true or false to the blackboard key depending on whether or not the player is in range
	const bool bIsInRange{NPC->GetDistanceTo(Player) <= MeleeRange};
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsInRange);
}
