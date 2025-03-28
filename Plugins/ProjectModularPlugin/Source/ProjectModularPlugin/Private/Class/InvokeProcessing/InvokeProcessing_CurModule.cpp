// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_CurModule.h"

#include "Class/ModuleEventCallableInterface.h"
#include "Class/RuleProcessingClassBase.h"


void UInvokeProcessing_CurModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, 
	const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleEventCallableInterface::Execute_GetModuleName(ModuleComponent->GetOwner());
	AActor* ModuleActor = ModuleComponent->GetOwner();

	FString PreviousModuleName;
	if (PreviousComponent != nullptr)
	{
		PreviousModuleName = IModuleEventCallableInterface::Execute_GetModuleName(PreviousComponent->GetOwner());	
	}
	else
	{
		PreviousModuleName = TEXT("Null Module");
	}

	IModuleEventCallableInterface::Execute_ModuleEvent(ModuleActor, ModuleName, PreviousModuleName, Rule.RuleName);
	
	for (TScriptInterface<IModuleEventCallableInterface> BinderClassInterface : ModuleComponent->GetInvokeEventBinders())
	{
		IModuleEventCallableInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, PreviousModuleName, Rule.RuleName);
	}
	
}
