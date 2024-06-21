// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "NPC_AIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class CPPAITUTORIAL_API ANPC_AIController : public AAIController
{
	GENERATED_BODY()

public:
	explicit ANPC_AIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

#pragma region Perception

private:
	void SetupPerceptionSystem();
	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);

	TObjectPtr<UAISenseConfig_Sight> SightConfig;

#pragma endregion
};
