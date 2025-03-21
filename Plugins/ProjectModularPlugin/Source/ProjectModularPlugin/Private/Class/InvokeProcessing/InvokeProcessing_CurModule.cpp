// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_CurModule.h"

#include "Class/ModuleClassInterface.h"
#include "Class/RuleProcessingClassBase.h"


void UInvokeProcessing_CurModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent,
	const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleClassInterface::Execute_GetModuleName(ModuleComponent->GetOwner());
	AActor* ModuleActor = ModuleComponent->GetOwner();

	IModuleClassInterface::Execute_ModuleEvent(ModuleActor, ModuleName, TEXT(""), Rule.RuleName);
	
	for (TScriptInterface<IModuleClassInterface> BinderClassInterface : ModuleComponent->GetInvokeEventBinders())
	{
		IModuleClassInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, TEXT(""), Rule.RuleName);
	}
	
}
