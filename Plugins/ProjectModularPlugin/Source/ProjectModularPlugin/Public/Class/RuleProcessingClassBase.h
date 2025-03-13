// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuleProcessingClassBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol UMETA(DisplayName = "Pistol"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	RocketLauncher UMETA(DisplayName = "Rocket Launcher")
};

USTRUCT(BlueprintType)
struct FRule
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType CullDistance;
};

UCLASS()
class PROJECTMODULARPLUGIN_API ARuleProcessingClassBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARuleProcessingClassBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<FRule> Rules;
	
};
