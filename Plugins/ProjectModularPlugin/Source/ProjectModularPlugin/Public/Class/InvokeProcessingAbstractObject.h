// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularTypes.h"
#include "InvokeProcessingAbstractObject.generated.h"

class IModuleEventCallableInterface;
class UModuleComponent;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PROJECTMODULARPLUGIN_API UInvokeProcessingAbstractObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void InvokeProcessing(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, FRule Rule);
	virtual void InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, FRule Rule);
	
};
