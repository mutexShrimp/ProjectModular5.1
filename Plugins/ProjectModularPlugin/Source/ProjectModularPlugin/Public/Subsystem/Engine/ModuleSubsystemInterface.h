// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Class/ModuleClassInterface.h"
#include "UObject/Interface.h"
#include "ModuleSubsystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModuleSubsystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMODULARPLUGIN_API IModuleSubsystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void BindModule(const TScriptInterface<IModuleClassInterface>& ModuleClass);

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	// void BindInvokeEvent(const TScriptInterface<IModuleClassInterface>& ModuleClass);
	
};
