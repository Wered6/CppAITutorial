// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"
#include "AIController.h"
#include "NPC.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// if we are out of range, do not attack the player
	const bool OutOfRange{!OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey())};
	if (OutOfRange)
	{
		// finish the task
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	// we are in range so get the AI's controller and the NPC itself
	const AAIController* const AIC{OwnerComp.GetAIOwner()};

#pragma region Nullchecks
	if (!AIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|AIC is nullptr"), *FString(__FUNCTION__))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	ANPC* const NPC{Cast<ANPC>(AIC->GetPawn())};

#pragma region Nullchecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|NPC is nullptr"), *FString(__FUNCTION__))
		return EBTNodeResult::Failed;
	}
#pragma endregion

	// if the NPC supports the ICombatInterface, cast and call the Execute_MeleeAttack function
	const bool bImplementsInterface{NPC->Implements<UCombatInterface>()};
	if (bImplementsInterface)
	{
		// necessary check to see if the montage has finished so we don't try and play it again
		if (MontageHasFinished(NPC))
		{
			ICombatInterface::Execute_MeleeAttack(NPC);
		}
	}

	// finish the task
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}

bool UBTTask_MeleeAttack::MontageHasFinished(const ANPC* const NPC) const
{
#pragma region Nullchecks
	if (!NPC)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|NPC is nullptr"), *FString(__FUNCTION__))
		return false;
	}
#pragma endregion

	const UAnimMontage* Montage{NPC->GetMontage()};

#pragma region Nullchecks
	if (!Montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|Montage is nullptr"), *FString(__FUNCTION__))
		return false;
	}
#pragma endregion

	const bool bHasMontageFinished{NPC->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Montage)};

	return bHasMontageFinished;
}
