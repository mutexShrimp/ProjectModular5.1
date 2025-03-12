// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleComponentBase.h"
#include "Components/ActorComponent.h"
#include "ModuleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROJECTMODULARPLUGIN_API UModuleComponent : public UModuleComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UModuleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
};
