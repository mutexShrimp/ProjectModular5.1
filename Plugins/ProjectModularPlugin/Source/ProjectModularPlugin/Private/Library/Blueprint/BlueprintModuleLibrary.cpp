// Copyright Shrimp Chen 2025 All Rights Reserved.


#include "Library/Blueprint/BlueprintModuleLibrary.h"
#include "ProjectModularPlugin.h"
#include "Subsystem/World/ModuleSubsystem.h"

UBlueprintModuleLibrary::UBlueprintModuleLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UBlueprintModuleLibrary::IsBlueprintFunctionImplemented(UObject* Object, FName FunctionName)
{
	if (Object)
	{
		UClass* ObjectClass = Object->GetClass();
		UFunction* Function = ObjectClass->FindFunctionByName(FunctionName);
		if (Function && Function->HasAnyFunctionFlags(FUNC_Native))
		{
			return false; // 函数是原生的，未被蓝图覆盖
		}
		return true; // 函数被蓝图覆盖
	}
	return false; // 对象无效
}
