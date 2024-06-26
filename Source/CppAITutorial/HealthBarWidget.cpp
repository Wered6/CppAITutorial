// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealthBarWidget::SetBarValuePercent(float const Value) const
{
	HealthValue->SetPercent(Value);
}
