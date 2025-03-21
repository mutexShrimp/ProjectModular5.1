// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ModuleComponent.h"
#include "Component/ModuleComponentInterface.h"
#include "Subsystem/World/ModuleSubsystem.h"
#include "Subsystem/World/ModuleSubsystemInterface.h"

// Sets default values for this component's properties
UModuleComponent::UModuleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UModuleComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		if (GetOwner()->GetClass()->ImplementsInterface(UModuleClassInterface::StaticClass()))
		{
			UModuleSubsystem* ModuleSubsystem = World->GetSubsystem<UModuleSubsystem>();
			TScriptInterface<IModuleClassInterface> ModuleClassInterface(GetOwner());
			IModuleSubsystemInterface::Execute_BindModule(ModuleSubsystem, ModuleClassInterface);
			//IModuleComponentInterface::Execute_BindInvokeEvent(this, ModuleClassInterface);
			
		}
	}
}

// Called every frame
void UModuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

