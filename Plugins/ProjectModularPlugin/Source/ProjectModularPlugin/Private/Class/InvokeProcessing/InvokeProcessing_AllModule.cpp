// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_AllModule.h"
#include "Class/ModuleClassInterface.h"
#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"

void UInvokeProcessing_AllModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, FRule Rule)
{
	TArray<TScriptInterface<IModuleClassInterface>> ModuleClasses = ModuleComponent->GetInvokeEventBinders();
	for (TScriptInterface<IModuleClassInterface> ModuleClass : ModuleClasses)
	{
		UE_LOG(LogTemp, Warning, TEXT("InvokeProcessing Function is Invoked by %s"), *ModuleClass.GetObject()->GetName());
		IModuleClassInterface::Execute_ModuleEvent(ModuleClass.GetObject(), Rule.RuleName);
	}
}
