// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/World/ModuleSubsystem.h"

#include "Class/RuleProcessingClassBase.h"
#include "Component/ModuleComponent.h"


UModuleSubsystem::UModuleSubsystem()
{
	
}

void UModuleSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	Reset();
	
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

void UModuleSubsystem::BindModule_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass)
{
	UE_LOG(LogTemp, Warning, TEXT("BindModule Function is Invoked by %s"), *ModuleClass.GetObject()->GetName());

	FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
	Modules.Add(ModuleName, ModuleClass);

	UE_LOG(LogTemp, Warning, TEXT("Added %s Module"), *ModuleName);
}

void UModuleSubsystem::InvokeModuleByName_Implementation(const FString& ModuleName, bool& isEmpty)
{
	InvokeModule(*Modules.Find(ModuleName), isEmpty);
}

void UModuleSubsystem::InvokeModuleByClass_Implementation(const TScriptInterface<IModuleClassInterface>& ModuleClass,
	bool& isEmpty)
{
	InvokeModule(ModuleClass, isEmpty);
}

void UModuleSubsystem::InvokeModule(const TScriptInterface<IModuleClassInterface>& ModuleClass, bool& isEmpty)
{
	UE_LOG(LogTemp, Warning, TEXT("InvokeModule Function is Invoked by %s"), *ModuleClass.GetObject()->GetName());

	FString ModuleName = ModuleClass->Execute_GetModuleName(ModuleClass.GetObject());
	TScriptInterface<IModuleClassInterface>* ModuleClass1 = Modules.Find(ModuleName);
	isEmpty = !IsValid(ModuleClass1->GetObject());
	if (!isEmpty)
	{
		TArray<FString> KeysArray;
		int32 KeysNum = Modules.GetKeys(KeysArray);
		for (TArray<FString>::TIterator It(KeysArray); It; ++It)
		{
			FString& Key = *It;
			TScriptInterface<IModuleClassInterface>* ModuleClass2 = Modules.Find(Key);
			AActor* Actor = Cast<AActor>(ModuleClass2->GetObject());
			UModuleComponent* Component = Actor->FindComponentByClass<UModuleComponent>();
			if (Component && RuleProcessingClassArr.Num() > 0)
			{
				for (ARuleProcessingClassBase* ProcessingClassArr : RuleProcessingClassArr)
				{
					ProcessingClassArr->HandleInvocation(Component);
				}
			}
		}
	}
}

void UModuleSubsystem::Reset()
{
	Modules.Empty();
	
	RuleProcessingClassArr.Empty();
	
}




