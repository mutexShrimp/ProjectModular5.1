// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_CurModule.h"

#include "Class/ModuleClassInterface.h"
#include "Class/RuleProcessingClassBase.h"


void UInvokeProcessing_CurModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, 
	const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleClassInterface::Execute_GetModuleName(ModuleComponent->GetOwner());
	AActor* ModuleActor = ModuleComponent->GetOwner();

	FString PreviousModuleName;
	if (PreviousComponent != nullptr)
	{
		PreviousModuleName = IModuleClassInterface::Execute_GetModuleName(PreviousComponent->GetOwner());	
	}
	else
	{
		PreviousModuleName = TEXT("Null Module");
	}

	IModuleClassInterface::Execute_ModuleEvent(ModuleActor, ModuleName, PreviousModuleName, Rule.RuleName);
	
	for (TScriptInterface<IModuleClassInterface> BinderClassInterface : ModuleComponent->GetInvokeEventBinders())
	{
		IModuleClassInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, PreviousModuleName, Rule.RuleName);
	}
	
}
