// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Class/InvokeProcessingAbstractObject.h"
#include "InvokeProcessing_CurModule.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMODULARPLUGIN_API UInvokeProcessing_CurModule : public UInvokeProcessingAbstractObject
{
	GENERATED_BODY()

public:
	virtual void InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, FRule Rule) override;
};
