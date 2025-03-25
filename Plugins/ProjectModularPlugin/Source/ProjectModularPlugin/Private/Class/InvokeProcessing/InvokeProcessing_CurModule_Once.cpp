// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_CurModule_Once.h"

#include "Class/ModuleClassInterface.h"
#include "Class/RuleProcessingClassBase.h"

TArray<FString> UInvokeProcessing_CurModule_Once::ModuleNameArr;
TArray<FString> UInvokeProcessing_CurModule_Once::ModuleRuleNameArr;

UInvokeProcessing_CurModule_Once::~UInvokeProcessing_CurModule_Once()
{
	ModuleNameArr.Empty();
	ModuleRuleNameArr.Empty();
}

void UInvokeProcessing_CurModule_Once::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent,
                                                                       const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleClassInterface::Execute_GetModuleName(ModuleComponent->GetOwner());

	bool bHasModuleRule = false;
	for (int32 i = 0; i < ModuleNameArr.Num(); i++)
	{
		if (ModuleNameArr[i] == ModuleName)
		{
			bHasModuleRule = ModuleRuleNameArr[i] == Rule.RuleName ? true : false;
			if (bHasModuleRule)
			{
				break;
			}
		}
	}

	if (!bHasModuleRule)
	{
		ModuleNameArr.Add(ModuleName);
		ModuleRuleNameArr.Add(Rule.RuleName);
		
		AActor* ModuleActor = ModuleComponent->GetOwner();

		IModuleClassInterface::Execute_ModuleEvent(ModuleActor, ModuleName, TEXT(""), Rule.RuleName);
	
		for (TScriptInterface<IModuleClassInterface> BinderClassInterface : ModuleComponent->GetInvokeEventBinders())
		{
			IModuleClassInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, TEXT(""), Rule.RuleName);
		}
	}
}
