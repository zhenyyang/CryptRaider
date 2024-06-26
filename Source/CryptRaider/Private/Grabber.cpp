// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	if (PhysicsHandle->GetGrabbedComponent() && PhysicsHandle) {
		FVector TargetLocation = GetComponentLocation() + HoldDistance * GetForwardVector();
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation,GetComponentRotation());
	}	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) {
		return;
	}
	FHitResult HitResult;
	bool HasHit = GetGrabberleInReach(HitResult);
	if (HasHit) {
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();		//ÉèÖÃÎïÀí±ú
		AActor* HitActor = HitResult.GetActor();
		HitActor->Tags.Add("Grabber");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),NAME_None, HitResult.ImpactPoint,GetComponentRotation()
		);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HitResult is hahahahh"));
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr) {
		return;
	}
	//UPrimitiveComponent* HitComponent =  PhysicsHandle->GetGrabbedComponent();
	if (PhysicsHandle->GetGrabbedComponent() != NULL) {
		AActor* GrabberActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabberActor->Tags.Remove("Grabber");
		PhysicsHandle->ReleaseComponent();
	}
	UE_LOG(LogTemp, Warning, TEXT("Release is hahahahh"));
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Grabber requires a UPhysicsHandleComponent"));
	}
	return Result;
}

bool UGrabber::GetGrabberleInReach(FHitResult& OutHit) const
{
	FVector StartLocation = GetComponentLocation();
	FVector EndLocation = StartLocation + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red);
	DrawDebugSphere(GetWorld(),EndLocation,10,10,FColor::Blue);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(OutHit, StartLocation, EndLocation,
		FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}

