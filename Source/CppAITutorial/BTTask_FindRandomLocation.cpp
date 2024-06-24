// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = "Find Random Location In NavMesh";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get AI controller and its npc
	const ANPC_AIController* const NPCAIC{Cast<ANPC_AIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!NPCAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindRandomLocation::ExecuteTask|NPCAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	const AActor* const NPC{NPCAIC->GetPawn()};

#pragma region NullChecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindRandomLocation::ExecuteTask|NPC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// obtain npc location to use as an origin
	const FVector ActorLocation{NPC->GetActorLocation()};

	// get the navigation system and generate a random location
	const UNavigationSystemV1* const NavSys{UNavigationSystemV1::GetCurrent(GetWorld())};

#pragma region NullChecks
	if (!NavSys)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNavigationSystemV1|NavSys is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	FNavLocation NavLocation;
	if (NavSys->GetRandomPointInNavigableRadius(ActorLocation, SearchRadius, NavLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
	}

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
