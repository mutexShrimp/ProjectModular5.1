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

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void BindModule(const TScriptInterface<IModuleClassInterface>& ModuleClass);
	virtual void BindModule_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void InvokeModuleByName(const FString& ModuleName, bool& isEmpty);
	virtual void InvokeModuleByName_Implementation(const FString& ModuleName, bool& isEmpty);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void InvokeModuleByClass(const TScriptInterface<IModuleClassInterface>& ModuleClass, bool& isEmpty);
	virtual void InvokeModuleByClass_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass, bool& isEmpty);

private:
	void InvokeModule(const TScriptInterface<IModuleClassInterface>& ModuleClass, bool& isEmpty);
	
protected:
	TMap<FString, TScriptInterface<IModuleClassInterface>> Modules;
	
};
