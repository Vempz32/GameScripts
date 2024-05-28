// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	// checks if the object has a Physics Handle and updates the targetLocation and rotation 
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
}


void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	// Checking if the actor has a PhysicsHandle
	if(PhysicsHandle == nullptr)
	{
		return;
	}
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	// if the geometery sweep connects with the actor grab the actor 
	if (HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		// Adding a "Grabbed" tag to the grabbed actor
		HitActor->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		// Grabbing the Actor at its impact point and keeping its rotation
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		); 
	}
	
}
void UGrabber::Release()
{
	// Using a pointer to reference the PhysicsHandle
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	// 
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle-> GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* GrabbedActor =PhysicsHandle-> GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle-> ReleaseComponent();
	}
}
	UPhysicsHandleComponent* UGrabber:: GetPhysicsHandle() const
	{
		UPhysicsHandleComponent* Result = GetOwner()-> FindComponentByClass<UPhysicsHandleComponent>(); 
		// Checking if the Actor has a physicsHandle component and logging and error if there isnt 
		if (Result == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Grabber requires a UphysicsHandleComponent"));
		}
		
		return Result;
		
	}

	bool UGrabber:: GetGrabbableInReach(FHitResult& OutHitResult) const
	{
		// Creating a Collison Sphere to Sweep and check if it collides with an actor
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	FCollisionShape Sphere = FCollisionShape:: MakeSphere(GrabRadius);
	 	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End, 
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere);
	}



