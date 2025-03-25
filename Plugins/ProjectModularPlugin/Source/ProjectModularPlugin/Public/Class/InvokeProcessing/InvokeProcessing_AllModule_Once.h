// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Class/InvokeProcessingAbstractObject.h"
#include "InvokeProcessing_AllModule_Once.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMODULARPLUGIN_API UInvokeProcessing_AllModule_Once : public UInvokeProcessingAbstractObject
{
	GENERATED_BODY()

public:
	~UInvokeProcessing_AllModule_Once() override;

	virtual void InvokeProcessing_Implementation(UModuleComponent* ModuleComponent,
	                                             const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule) override;

protected:
	static TArray<FString> ModuleNameArr;
	static TArray<FString> ModuleRuleNameArr;
	
};
