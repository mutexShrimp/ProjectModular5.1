// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_CurModule_Once.h"

#include "Class/ModuleEventCallableInterface.h"
#include "Class/RuleProcessingClassBase.h"

TArray<FString> UInvokeProcessing_CurModule_Once::ModuleNameArr;
TArray<FString> UInvokeProcessing_CurModule_Once::ModuleRuleNameArr;


UInvokeProcessing_CurModule_Once::~UInvokeProcessing_CurModule_Once()
{
	ModuleNameArr.Empty();
	ModuleRuleNameArr.Empty();
}

void UInvokeProcessing_CurModule_Once::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, 
                                                                       const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleEventCallableInterface::Execute_GetModuleName(ModuleComponent->GetOwner());
	FString PreviousModuleName;
	if (PreviousComponent != nullptr)
	{
		PreviousModuleName = IModuleEventCallableInterface::Execute_GetModuleName(PreviousComponent->GetOwner());	
	}
	else
	{
		PreviousModuleName = TEXT("Null Module");
	}
	
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

		IModuleEventCallableInterface::Execute_ModuleEvent(ModuleActor, ModuleName, PreviousModuleName, Rule.RuleName);
	
		for (TScriptInterface<IModuleEventCallableInterface> BinderClassInterface : ModuleComponent->GetInvokeEventBinders())
		{
			IModuleEventCallableInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, PreviousModuleName, Rule.RuleName);
		}
	}
}

