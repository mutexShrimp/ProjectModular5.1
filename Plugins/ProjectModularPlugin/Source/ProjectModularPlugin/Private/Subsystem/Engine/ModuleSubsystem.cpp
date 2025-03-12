// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/Engine/ModuleSubsystem.h"



// void UModuleSubsystem::BindModule(TScriptInterface<IModuleClassInterface> Module)
// {
// 	IModuleClassInterface* ModulePtr = Module.GetInterface();
// 	if (ModulePtr)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("BindModule 被调用，模块已绑定"));
// 	}
// }

UModuleSubsystem::UModuleSubsystem()
{
}

void UModuleSubsystem::BindModule_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Function is Invoked by %s"), __FUNCTION__, *ModuleClass.GetObject()->GetName());

	FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
	Modules.Add(ModuleName, ModuleClass);

	UE_LOG(LogTemp, Warning, TEXT("Added %s Module"), *ModuleName);
}

// void UModuleSubsystem::BindInvokeEvent_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass)
// {
// 	//IModuleSubsystemInterface::BindInvokeEvent_Implementation(ModuleClass);
// 	
// }


