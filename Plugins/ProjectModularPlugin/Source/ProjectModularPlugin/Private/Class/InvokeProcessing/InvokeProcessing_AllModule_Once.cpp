// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_AllModule_Once.h"

#include "Class/ModuleClassInterface.h"
#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"

TArray<FString> UInvokeProcessing_AllModule_Once::ModuleNameArr;
TArray<FString> UInvokeProcessing_AllModule_Once::ModuleRuleNameArr;

UInvokeProcessing_AllModule_Once::~UInvokeProcessing_AllModule_Once()
{
	ModuleNameArr.Empty();
	ModuleRuleNameArr.Empty();
}

void UInvokeProcessing_AllModule_Once::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent,
                                                                       const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleClassInterface::Execute_GetModuleName(ModuleComponent->GetOwner());
	TArray<UObject*> ModuleObjects;
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
		
		for (TTuple<FString, TScriptInterface<IModuleClassInterface>> NewModule : NewModules)
		{
			TScriptInterface<IModuleClassInterface> ModuleClassInterface = NewModule.Get<1>();

			IModuleClassInterface::Execute_ModuleEvent(ModuleClassInterface.GetObject(), ModuleName, TEXT(""), Rule.RuleName);

			UModuleComponentBase* L_ModuleComponent = Cast<AActor>(ModuleClassInterface.GetObject())->FindComponentByClass<UModuleComponentBase>();
			if (L_ModuleComponent)
			{
				for (TScriptInterface<IModuleClassInterface> BinderClassInterface : L_ModuleComponent->GetInvokeEventBinders())
				{
					// 处理同个对象绑定多个模块多次实现相同事件问题
					if (!(ModuleObjects.Contains(BinderClassInterface.GetObject())))
					{
						ModuleObjects.Add(BinderClassInterface.GetObject());
			
						// 执行事件
						IModuleClassInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, TEXT(""), Rule.RuleName);	
					}
				}
			}
		}
	}
	
}
