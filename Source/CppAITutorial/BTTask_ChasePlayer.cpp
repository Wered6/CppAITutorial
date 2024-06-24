// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "NPC_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& ObjectInitializer) : UBTTask_BlackboardBase(
	ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get target location from blackboard via the NPC's controller
	ANPC_AIController* const NPCAIC{Cast<ANPC_AIController>(OwnerComp.GetAIOwner())};

#pragma region NullChecks
	if (!NPCAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer::ExecuteTask|NPCAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	FVector const PlayerLocation{OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey())};

	// move to the player's location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(NPCAIC, PlayerLocation);

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
