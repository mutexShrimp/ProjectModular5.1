// Copyright Shrimp Chen 2025 All Rights Reserved.


#include "Subsystem/World/ModuleSubsystem.h"

#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"
#include "Math/FloatPacker.h"
#include "Engine/World.h"

UModuleSubsystem::UModuleSubsystem()
{
	
}

void UModuleSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	Reset();

	UE_LOG(LogTemp, Warning, TEXT("Modular Project Plugin Default event invoke names include the following.\n"));
	UE_LOG(LogTemp, Display, TEXT("----------\n"));
	UE_LOG(LogTemp, Warning, TEXT("ModuleInitializationBegins_InvokeAll\n"));
	UE_LOG(LogTemp, Warning, TEXT("DoOnce_PreInitModule_InvokeAll\n"));
	UE_LOG(LogTemp, Warning, TEXT("DoOnce_PreInitModule\n"));
	UE_LOG(LogTemp, Warning, TEXT("InitModule\n"));
	UE_LOG(LogTemp, Warning, TEXT("DoOnce_PostInitModule_InvokeAll\n"));
	UE_LOG(LogTemp, Warning, TEXT("DoOnce_PostInitModule\n"));
	UE_LOG(LogTemp, Warning, TEXT("ModuleInitializationCompleted_InvokeAll\n"));
	UE_LOG(LogTemp, Display, TEXT("----------\n"));
	
	if (UWorld* World = GetWorld())
	{
		TArray<UObject*> FoundObjects;
		GetObjectsOfClass(ARuleProcessingClassBase::StaticClass(), FoundObjects, true);
		TMap<UClass*, TArray<UObject*>> Classes;
		
		for (UObject* FoundObject : FoundObjects)
		{
			if (FoundObject->GetWorld())
			{
				if (Classes.Contains(FoundObject->GetClass()))
				{
					Classes.Find(FoundObject->GetClass())->Add(FoundObject);
				}
				else
				{
					TArray<UObject*> Objs;
					Objs.Add(FoundObject);
					Classes.Add(FoundObject->GetClass(), Objs);
				}
			}
		}
		
		if (Classes.IsEmpty())
		{
			ARuleProcessingClassBase* NewActor = World->SpawnActor<ARuleProcessingClassBase>(ARuleProcessingClassBase::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			TArray<UObject*> Objs;
			Objs.Add(NewActor);
			Classes.Add(NewActor->GetClass(), Objs);
		}

		TArray<UClass*> FoundClasses;
		Classes.GetKeys(FoundClasses);
		UClass* FoundClass = FoundClasses[0];
		UE_LOG(LogTemp, Warning, TEXT("RuleProcessingClass Using %s"), *FoundClass->GetName());
		TArray<UObject*>* Objects = Classes.Find(FoundClass);
		if (Objects)
		{
			for (UObject* Obj : *Objects)
			{
				UE_LOG(LogTemp, Display, TEXT("Found RuleProcessingClass Name : %s"), *Obj->GetName());
				RuleProcessingClassArr.Add(Cast<ARuleProcessingClassBase>(Obj));
			}
		}
		
		if (Classes.Num() > 1)
		{
			UE_LOG(LogTemp, Error, TEXT("Too many RuleProcessingClasses (%d Classes) causing confusion"), Classes.Num());
		}
		
	}
}

void UModuleSubsystem::BeginDestroy()
{
	Super::BeginDestroy();
	
}

void UModuleSubsystem::BindModule_Implementation(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass)
{
	UE_LOG(LogTemp, Warning, TEXT("BindModule Function is Invoked by \"%s\""), *ModuleClass.GetObject()->GetName());

	UModuleComponentBase* ModuleComponent = Cast<AActor>(ModuleClass.GetObject())->FindComponentByClass<UModuleComponentBase>();

	if (ModuleComponent)
	{
		FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
		Modules.Add(ModuleName, ModuleClass);

		UE_LOG(LogTemp, Warning, TEXT("Added \"%s\" Module"), *ModuleName);	
	}
}

void UModuleSubsystem::InvokeModuleByName_Implementation(const FString& ModuleName, bool& isEmpty)
{
	InvokeModule(*Modules.Find(ModuleName), Modules,isEmpty);
}

void UModuleSubsystem::InvokeModuleByClass_Implementation(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass,
	bool& isEmpty)
{
	InvokeModule(ModuleClass, Modules, isEmpty);
}

UModuleComponentBase* UModuleSubsystem::GetModuleByName_Implementation(const FString& ModuleName, bool& isEmpty)
{
	TScriptInterface<IModuleEventCallableInterface>* ModuleClass = Modules.Find(ModuleName);
	if (ModuleClass)
	{
		UModuleComponentBase* ModuleComponent = Cast<AActor>(ModuleClass->GetObject())->FindComponentByClass<UModuleComponentBase>();
		if (ModuleComponent)
		{
			isEmpty = true;
			return ModuleComponent;
		}
	}
	isEmpty = false;
	return nullptr;
}

void UModuleSubsystem::InvokeModule(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass, const TMap<FString, TScriptInterface<IModuleEventCallableInterface>>& NewModules, bool& isEmpty)
{
	UE_LOG(LogTemp, Warning, TEXT("InvokeModule Function is Invoked by \"%s\""), *ModuleClass.GetObject()->GetName());

	FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
	TScriptInterface<IModuleEventCallableInterface>* ModuleClassInterface = Modules.Find(ModuleName);
	isEmpty = !IsValid(ModuleClassInterface->GetObject());
	if (!isEmpty)
	{
		AActor* ModuleActor = Cast<AActor>(ModuleClassInterface->GetObject());
		UModuleComponent* Component = ModuleActor->FindComponentByClass<UModuleComponent>();
		if (Component && RuleProcessingClassArr.Num() > 0)
		{
			TArray<UClass*> Classes;
			for (ARuleProcessingClassBase* ProcessingClass : RuleProcessingClassArr)
			{
				if (ProcessingClass->GetWorld() != nullptr)
				{
					if (!(Classes.Contains(ProcessingClass->GetClass())))
					{
						if (ProcessingClass->HandleInvocation(Component, PreviousComponent, NewModules))
						{
							Classes.Add(ProcessingClass->GetClass());	
						}
					}
				}
			}
		}
	}
}

void UModuleSubsystem::Reset()
{
	Modules.Empty();
	
	RuleProcessingClassArr.Empty();

	PreviousComponent = nullptr;
}




