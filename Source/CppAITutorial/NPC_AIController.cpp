// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"
#include "NPC.h"
#include "BehaviorTree/BehaviorTree.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
}

void ANPC_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const ANPC* const NPC{Cast<ANPC>(InPawn)};

#pragma region NullChecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPC_AIController::OnPossess|NPC is nullptr"))
		return;
	}
#pragma endregion

	UBehaviorTree* const Tree{NPC->GetBehaviorTree()};

#pragma region NullChecks
	if (!Tree)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPC_AIController::OnPossess|Tree is nullptr"))
		return;
	}
#pragma endregion

	UBlackboardComponent* BlackboardComponent;
	UseBlackboard(Tree->BlackboardAsset, BlackboardComponent);
	Blackboard = BlackboardComponent;
	RunBehaviorTree(Tree);
}
