// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& ObjectInitializer) :
	UBTTask_BlackboardBase(ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// get player character
	const ACharacter* Player{UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)};

#pragma region NullChecks
	if (!Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayerLocation::ExecuteTask|Player is nullptr"))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// get player location to use as an origin
	const FVector PlayerLocation{Player->GetActorLocation()};
	if (SearchRandom)
	{
		FNavLocation NavLocation;

		// get the navigation system and generate a random location near the player
		const UNavigationSystemV1* const NavSys{UNavigationSystemV1::GetCurrent(GetWorld())};

#pragma region NullChecks
		if (!NavSys)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayerLocation::ExecuteTask|NavSys is nullptr"))
			return EBTNodeResult::Failed;
		}
#pragma endregion

		// try to get a random location near the player
		const bool bFoundPlayerLocation{
			NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, NavLocation)
		};
		if (bFoundPlayerLocation)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerLocation);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
