// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_AllModule.h"
#include "Class/ModuleClassInterface.h"
#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"

void UInvokeProcessing_AllModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent,
	const TMap<FString, TScriptInterface<IModuleClassInterface>>& NewModules, FRule Rule)
{
	FString ModuleName = IModuleClassInterface::Execute_GetModuleName(ModuleComponent->GetOwner());

	TArray<UObject*> ModuleObjects;
	for (TTuple<FString, TScriptInterface<IModuleClassInterface>> NewModule : NewModules)
	{
		TScriptInterface<IModuleClassInterface> ModuleClassInterface = NewModule.Get<1>();

		IModuleClassInterface::Execute_ModuleEvent(ModuleClassInterface.GetObject(), ModuleName, TEXT(""), Rule.RuleName);

		UModuleComponentBase* L_ModuleComponent = Cast<AActor>(ModuleClassInterface.GetObject())->FindComponentByClass<UModuleComponentBase>();
		if (L_ModuleComponent)
		{
			for (TScriptInterface<IModuleClassInterface> BinderClassInterface : L_ModuleComponent->GetInvokeEventBinders())
			{
				// 处理同个对象绑定多个模块后多次调用相同事件问题
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
