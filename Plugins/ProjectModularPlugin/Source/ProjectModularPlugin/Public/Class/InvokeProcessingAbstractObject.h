// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvokeProcessingAbstractObject.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PROJECTMODULARPLUGIN_API UInvokeProcessingAbstractObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void InvokeProcessing(UModuleComponent* ModuleComponent, FRule Rule);
	virtual void InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, FRule Rule);
	
};
