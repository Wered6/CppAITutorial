// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IncrementPathIndex.h"
#include "NPC.h"
#include "NPC_AIController.h"
#include "PatrolPath.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_IncrementPathIndex::UBTTask_IncrementPathIndex(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UBTTask_IncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// try and get the AI controller
	const ANPC_AIController* const NPCAIC{Cast<ANPC_AIController>(OwnerComp.GetAIOwner())};
	// tey and get the blackboard
	UBlackboardComponent* const BlackboardComponent{OwnerComp.GetBlackboardComponent()};

#pragma region NullChecks
	if (!NPCAIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_IncrementPathIndex::ExecuteTask|NPCAIC is nullptr"))
		return EBTNodeResult::Failed;
	}
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_IncrementPathIndex::ExecuteTask|BlackboardComponent is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// try and get the NPC
	const ANPC* const NPC{Cast<ANPC>(NPCAIC->GetPawn())};

#pragma region NullChecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_IncrementPathIndex::ExecuteTask|NPC is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// try and get the patrol path
	const APatrolPath* PatrolPath{NPC->GetPatrolPath()};

#pragma region NullChecks
	if (!PatrolPath)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_IncrementPathIndex::ExecuteTask|PatrolPath is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// get number of patrol points and min and max indices
	int32 const NumberOfPoints{PatrolPath->Num()};
	constexpr int32 MinIndex{0};
	int32 const MaxIndex{NumberOfPoints - 1};
	int32 Index{BlackboardComponent->GetValueAsInt(GetSelectedBlackboardKey())};

	// change direciton if we are at the first or last index if we are in bidirectional mode
	if (bBiDirectional)
	{
		if (Index >= MaxIndex && Direction == EDirectionType::Forward)
		{
			Direction = EDirectionType::Reverse;
		}
		else if (Index == MinIndex && Direction == EDirectionType::Reverse)
		{
			Direction = EDirectionType::Forward;
		}
	}

	// write new value of index to blackboad
	int32 const NewIndex{(Direction == EDirectionType::Forward ? ++Index : --Index) % NumberOfPoints};
	BlackboardComponent->SetValueAsInt(GetSelectedBlackboardKey(), NewIndex);

	// finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
