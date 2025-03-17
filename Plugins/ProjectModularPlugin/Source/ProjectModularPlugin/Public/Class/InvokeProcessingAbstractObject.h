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

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void InvokeProcessing();
	virtual void InvokeProcessing_Implementation();
	
};
