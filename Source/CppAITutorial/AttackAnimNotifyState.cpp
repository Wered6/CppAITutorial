// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAnimNotifyState.h"
#include "CppAICharacterBase.h"

void UAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration)
{
#pragma region Nullchecks
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|MeshComp is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	AActor* Owner{MeshComp->GetOwner()};

#pragma region Nullchecks
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|Owner is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	const ACppAICharacterBase* const CharacterBase{Cast<ACppAICharacterBase>(Owner)};

#pragma region Nullchecks
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|CharacterBase is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	CharacterBase->AttackStart();
}

void UAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
#pragma region Nullchecks
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|MeshComp is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	AActor* Owner{MeshComp->GetOwner()};

#pragma region Nullchecks
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|Owner is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	const ACppAICharacterBase* const CharacterBase{Cast<ACppAICharacterBase>(Owner)};

#pragma region Nullchecks
	if (!CharacterBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s|CharacterBase is nullptr"), *FString(__FUNCTION__))
		return;
	}
#pragma endregion

	CharacterBase->AttackEnd();
}
