// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPathPoint.h"
#include "NPC.h"
#include "NPC_AIController.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindPathPoint::UBTTask_FindPathPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Find Path Point");
}

EBTNodeResult::Type UBTTask_FindPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// attempt to get the NPC's controller
	const ANPC_AIController* const NPCAIC{Cast<ANPC_AIController>(OwnerComp.GetAIOwner())};
	// attempt to get the blackboard component from the behavior tree
	UBlackboardComponent* const BlackboardComponent{OwnerComp.GetBlackboardComponent()};

#pragma region NullChecks
	if (!NPCAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPathPoint::ExecuteTask|NPCAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPathPoint::ExecuteTask|BlackboardComponent is nullptr"));
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// get the current aptrol path index from the blackboard
	int32 const Index{BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey())};

	// attempt to get the NPC
	const ANPC* NPC{Cast<ANPC>(NPCAIC->GetPawn())};

#pragma region NullChecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPathPoint::ExecuteTask|NPC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// attempt to get the patrol path
	const APatrolPath* PatrolPath{NPC->GetPatrolPath()};

#pragma region NullChecks
	if (!PatrolPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPathPoint::ExecuteTask|PatrolPath is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// get the current patrol path vector from the NPC - this is local to the patrol path actor
	FVector const PatrolPoint{PatrolPath->GetPatrolPoint(Index)};

	// convert the local vector to a global point
	UE::Math::TVector const GlobalPoint{PatrolPath->GetActorTransform().TransformPosition(PatrolPoint)};
	BlackboardComponent->SetValueAsVector(PatrolPathVectorKey.SelectedKeyName, GlobalPoint);

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
