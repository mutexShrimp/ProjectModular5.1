// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintModuleLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMODULARPLUGIN_API UBlueprintModuleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsBlueprintFunctionImplemented", Keywords = "Is Blueprint Function Implemented"), Category = "Function")
	static bool IsBlueprintFunctionImplemented(UObject* Object, FName FunctionName);
	
};


