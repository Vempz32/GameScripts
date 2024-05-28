// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{

public:
	UTriggerComponent();
	
	GENERATED_BODY()
protected: 
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Creating a BluePrint Callabe function called SetMover
	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* Mover);


private:
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	UMover* Mover;

	AActor* GetAccetableActor() const;
};