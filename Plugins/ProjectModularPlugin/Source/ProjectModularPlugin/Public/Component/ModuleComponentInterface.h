// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ModuleComponentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModuleComponentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMODULARPLUGIN_API IModuleComponentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Function")
	void BindInvokeEvent(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass);
	
	
	
};
