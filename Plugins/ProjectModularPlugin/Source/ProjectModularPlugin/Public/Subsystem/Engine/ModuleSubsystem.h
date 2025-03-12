// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleSubsystemInterface.h"
#include "Subsystems/WorldSubsystem.h"
#include "ModuleSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMODULARPLUGIN_API UModuleSubsystem : public UWorldSubsystem,
	public IModuleSubsystemInterface
{
	GENERATED_BODY()

public:
	UModuleSubsystem();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void BindModule(const TScriptInterface<IModuleClassInterface>& ModuleClass);
	virtual void BindModule_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass);

	// UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	// void BindInvokeEvent(const TScriptInterface<IModuleClassInterface>& ModuleClass);
	// virtual void BindInvokeEvent_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass);
	
protected:
	TMap<FString, TScriptInterface<IModuleClassInterface>> Modules;
	
};
