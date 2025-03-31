// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Class/InvokeProcessingAbstractObject.h"
#include "InvokeProcessing_AllModule.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMODULARPLUGIN_API UInvokeProcessing_AllModule : public UInvokeProcessingAbstractObject
{
	GENERATED_BODY()

public:
	virtual void InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, 
		const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, FRule Rule) override;
};


