// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_CurModule_Once.h"

#include "Class/RuleProcessingClassBase.h"



void UInvokeProcessing_CurModule_Once::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent,
	const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule)
{
	Super::InvokeProcessing_Implementation(ModuleComponent, NewModules, Rule);
}
