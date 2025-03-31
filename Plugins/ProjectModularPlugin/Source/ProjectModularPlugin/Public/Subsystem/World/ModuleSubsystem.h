// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleSubsystemInterface.h"
#include "Subsystems/WorldSubsystem.h"
#include "ModuleSubsystem.generated.h"

class ARuleProcessingClassBase;

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
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void BindModule(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass);
	virtual void BindModule_Implementation(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void InvokeModuleByName(const FString& ModuleName, bool& isEmpty);
	virtual void InvokeModuleByName_Implementation(const FString& ModuleName, bool& isEmpty);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void InvokeModuleByClass(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass, bool& isEmpty);
	virtual void InvokeModuleByClass_Implementation(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass, bool& isEmpty);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	UModuleComponentBase* GetModuleByName(const FString& ModuleName, bool& isEmpty);
	virtual UModuleComponentBase* GetModuleByName_Implementation(const FString& ModuleName, bool& isEmpty);
	
private:
	void InvokeModule(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, bool& isEmpty);

	void Reset();
	
protected:
	TMap<FString, TScriptInterface<IModuleEventCallableInterface>> Modules;
	
	TArray<ARuleProcessingClassBase*> RuleProcessingClassArr;

	UPROPERTY()
	UModuleComponent* PreviousComponent = nullptr;
	
};
