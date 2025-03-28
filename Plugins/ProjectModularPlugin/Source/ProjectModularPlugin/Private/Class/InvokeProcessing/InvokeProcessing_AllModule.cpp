// Fill out your copyright notice in the Description page of Project Settings.


#include "Class/InvokeProcessing/InvokeProcessing_AllModule.h"
#include "Class/ModuleEventCallableInterface.h"
#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"

void UInvokeProcessing_AllModule::InvokeProcessing_Implementation(UModuleComponent* ModuleComponent, UModuleComponent* PreviousComponent, 
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

	TArray<UObject*> ModuleObjects;
	for (TTuple<FString, TScriptInterface<IModuleEventCallableInterface>> NewModule : NewModules)
	{
		TScriptInterface<IModuleEventCallableInterface> ModuleClassInterface = NewModule.Get<1>();

		IModuleEventCallableInterface::Execute_ModuleEvent(ModuleClassInterface.GetObject(), ModuleName, PreviousModuleName, Rule.RuleName);

		UModuleComponentBase* L_ModuleComponent = Cast<AActor>(ModuleClassInterface.GetObject())->FindComponentByClass<UModuleComponentBase>();
		if (L_ModuleComponent)
		{
			for (TScriptInterface<IModuleEventCallableInterface> BinderClassInterface : L_ModuleComponent->GetInvokeEventBinders())
			{
				// 处理同个对象绑定多个模块后多次调用相同事件问题
				if (!(ModuleObjects.Contains(BinderClassInterface.GetObject())))
				{
					ModuleObjects.Add(BinderClassInterface.GetObject());
			
					// 执行事件
					IModuleEventCallableInterface::Execute_ModuleEvent(BinderClassInterface.GetObject(), ModuleName, PreviousModuleName, Rule.RuleName);	
				}
			}
		}
		
	}
	
}
