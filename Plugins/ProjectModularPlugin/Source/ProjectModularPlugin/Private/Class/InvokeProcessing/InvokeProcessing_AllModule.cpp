// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_AllModule.h"

#include "Component/ModuleComponent.h"

void UInvokeProcessing_AllModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent)
{
	TArray<TScriptInterface<IModuleClassInterface>> ModuleClasses = ModuleComponent->GetInvokeEventBinders();
	for (TScriptInterface<IModuleClassInterface> ModuleClass : ModuleClasses)
	{
		UE_LOG(LogTemp, Warning, TEXT("InvokeProcessing Function is Invoked by %s"), *ModuleClass.GetObject()->GetName());
	}
}
