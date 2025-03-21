// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ModuleClassInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UModuleClassInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMODULARPLUGIN_API IModuleClassInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FString GetModuleName();
	virtual FString GetModuleName_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModuleEvent(const FString& ModuleName, const FString& PrevModuleName, const FString& EventName);
	virtual void ModuleEvent_Implementation(const FString& ModuleName, const FString& PrevModuleName, const FString& EventName);
};
