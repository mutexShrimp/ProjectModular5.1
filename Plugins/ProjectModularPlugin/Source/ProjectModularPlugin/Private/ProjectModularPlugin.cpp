// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectModularPlugin.h"

#include "Subsystem/Engine/ModuleSubsystem.h"

#define LOCTEXT_NAMESPACE "FProjectModularPluginModule"

void FProjectModularPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	// // 注册子系统
	// FWorldDelegates::OnPostWorldInitialization.AddLambda([](UWorld* World, const UWorld::InitializationValues IVS)
	// {
	// 	if (!World->GetSubsystem<UModuleSubsystem>())
	// 	{
	// 		World->GetSubsystemManager()->AddSubsystem<UModuleSubsystem>();
	// 	}
	// });
}

void FProjectModularPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProjectModularPluginModule, ProjectModularPlugin)