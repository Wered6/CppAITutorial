// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_AIController.h"
#include "CppAITutorialCharacter.h"
#include "NPC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ANPC_AIController::ANPC_AIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
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

void ANPC_AIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

#pragma region NullChecks
	if (!SightConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPC_AIController::SetupPerceptionSystem|SightConfig is nullptr"))
		return;
	}
#pragma endregion

	UAIPerceptionComponent* PerceptionComp{
		CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"))
	};
	SetPerceptionComponent(*PerceptionComp);

	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
	SightConfig->PeripheralVisionAngleDegrees = 90.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPC_AIController::OnTargetDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
}

void ANPC_AIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	const ACppAITutorialCharacter* const Player{Cast<ACppAITutorialCharacter>(Actor)};

#pragma region NullChecks
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANPC_AIController::OnTargetDetected|Player is nullptr"))
		return;
	}
#pragma endregion

	GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
}
