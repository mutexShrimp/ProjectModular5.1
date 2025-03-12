// Fill out your copyright notice in the Description page of Project Settings.

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
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "ProjectModularPlugin sample test testing"), Category = "ProjectModularPluginTesting")
	static float ProjectModularPluginSampleFunction(float Param);

	
};


