// Fill out your copyright notice in the Description page of Project Settings.

// Including the proper header files
#include "TriggerComponent.h"
#include "Components/PrimitiveComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}
void UTriggerComponent:: TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
   AActor* Actor = GetAccetableActor();
   // Checking if the Actor doesnt have a null pointer
   if(Actor != nullptr)
   {
    
    UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
    // If the component has a doesnt have a null pointer dont Set Simulate Physics to false
    if(Component != nullptr)
    {
       Component->SetSimulatePhysics(false);
    }
    //Attaching the actor to the trigger component
    Actor->AttachToComponent(this, FAttachmentTransformRules:: KeepWorldTransform);
    Mover->SetCanMove(true);
    // Else dont allow the Mover to move
   }else{
    Mover->SetCanMove(false);5
   }
}
// *********
void UTriggerComponent:: SetMover(UMover* NewMover)
{
    Mover = NewMover;
}


AActor* UTriggerComponent:: GetAccetableActor() const
{
    
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);

    for(AActor* Actor : Actors)
    {
        // Checing if the Actor has the Acceptable tag and it doesnt have the "Grabbed" Tag
        if(Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
        {
            return Actor;
        }
    }
    // Returning a null pointer
    return nullptr;
    
}


