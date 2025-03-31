// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Class/ModuleEventCallableInterface.h"
#include "UObject/Interface.h"
#include "ModuleSubsystemInterface.generated.h"

class UModuleComponentBase;
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
	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void BindModule(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void InvokeModuleByName(const FString& ModuleName, bool& isEmpty);

	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void InvokeModuleByClass(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass, bool& isEmpty);

	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	UModuleComponentBase* GetModuleByName(const FString& ModuleName, bool& isEmpty);

	// UFUNCTION(BlueprintNativeEvent, Category = "Function")
	// void BindInvokeEventToAllModule(const TScriptInterface<IModuleClassInterface>& ModuleClass);
	
};
