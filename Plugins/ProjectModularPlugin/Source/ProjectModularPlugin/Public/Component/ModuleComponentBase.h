// Copyright Shrimp Chen 2025 All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleComponentInterface.h"
#include "Components/ActorComponent.h"
#include "ModuleComponentBase.generated.h"


class IModuleEventCallableInterface;
UCLASS( ClassGroup=(Custom))
class PROJECTMODULARPLUGIN_API UModuleComponentBase : public UActorComponent,
	public IModuleComponentInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UModuleComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Function")
	void BindInvokeEvent(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass);
	virtual void BindInvokeEvent_Implementation(const TScriptInterface<IModuleEventCallableInterface>& ModuleClass);

public:
	TArray<TScriptInterface<IModuleEventCallableInterface>> GetInvokeEventBinders();
	
protected:
	TArray<TScriptInterface<IModuleEventCallableInterface>> InvokeEventBinders;
	
};
