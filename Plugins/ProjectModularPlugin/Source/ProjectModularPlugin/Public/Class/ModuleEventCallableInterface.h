// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ModuleEventCallableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModuleEventCallableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMODULARPLUGIN_API IModuleEventCallableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Module Event Callable | Function | Virtual",
		meta = (ToolTip =
			"If it is a module, it must be rewritten."
			))
	FString GetModuleName();
	virtual FString GetModuleName_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Module Event Callable | Function | Virtual",
		meta = (ToolTip =
			"The default event invoke names include the following.\n\nModuleInitializationBegins_InvokeAll\nDoOnce_PreInitModule_InvokeAll\nDoOnce_PreInitModule\nInitModule\nDoOnce_PostInitModule_InvokeAll\nDoOnce_PostInitModule\nModuleInitializationCompleted_InvokeAll\n"
			))
	void ModuleEvent(const FString& ModuleName, const FString& PrevModuleName, const FString& EventName);
	virtual void ModuleEvent_Implementation(const FString& ModuleName, const FString& PrevModuleName, const FString& EventName);
};
