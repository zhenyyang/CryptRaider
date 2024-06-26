// Fill out your copyright notice in the Description page of Project Settings.


#include "AttckComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "SHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UAttckComponent::UAttckComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_CurrentAttackActive = AttackMode::RangedAttack;
	m_ETraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	m_CurrentAttackSkill = AttackSkill::Rifle;

	m_FRadius = 150;
	AttackDistance_Y = 3000;
	AttackDistance_J = 1200;
	m_iAttackType = 1;

	Tags.Add(FName(""));
}


// Called when the game starts
void UAttckComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = Cast<USHealthComponent>(GetOwner()->GetComponentByClass(ComponentClass));

}


// Called every frame
void UAttckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FHitResult> UAttckComponent::RangedAttackRange(FVector StartLocation, FVector EndLocation)
{
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitArray;
	TArray<FHitResult> OutHits;
	FString HitTagEnemy;
	FString HitTagEnemyFilter;

	bool HitActor = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), EndLocation, EndLocation, m_FRadius,
		m_ETraceChannel, false, ActorsToIgnore, EDrawDebugTrace::Persistent,
		HitArray, true, FLinearColor::Red, FLinearColor::Green, 5);

	if (HitActor) {
		int32 len_tag1 = HitArray.Num();
		for (int i = 0; i < len_tag1; i++) {
			if (i == (len_tag1 - 1)) {
				break;
			}
			FHitResult Hit = HitArray[i];
			bool Tag_A = HitArray[i].GetActor()->Tags.Contains(Tags[0]);
			bool Tag_B = HitArray[i].GetActor()->Tags.Contains(Tags[1]);
			if (Tag_A) {
				OutHits.Add(Hit);
			}
			if (Tag_B) {
				HitTagEnemy = HitArray[i].GetActor()->Tags[1].ToString();
				HitTagEnemyFilter = HitArray[i + 1].GetActor()->Tags[1].ToString();
			}
		}
		
		int32 len_tag2 = OutHits.Num();
		for (int i = len_tag2 -1 ; i > 0; i--) {
			if (HitTagEnemy.Equals(HitTagEnemyFilter)) {
				OutHits.RemoveAt(i);
			}
		}
	}

	return OutHits;

}

void UAttckComponent::AttackEffect()
{
		
}

void UAttckComponent::GetEnemyLocation()
{
	TArray<AActor*> Actors;
	float tempD = 0.f;
	float minDistance = 0.f;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tags[1], Actors);
	if (!Actors.IsEmpty()) {
		minDistance = FMath::Abs(FVector::Distance(GetOwner()->GetActorLocation(), Actors[0]->GetActorLocation()));
		for (AActor* Actor : Actors) {
			tempD = FMath::Abs(FVector::Distance(GetOwner()->GetActorLocation(), Actor->GetActorLocation()));
			if (tempD < minDistance) {
				minDistance = tempD;
			}

			Actor = nullptr;
		}

		ACharacter* character = Cast<ACharacter>(Actors[0]);
		bool IsActive = character->GetCharacterMovement()->IsActive();
		if (IsActive && minDistance > 350) {
			minDistance = minDistance - 300 * Health->m_BasicDamage[0].SkillForwardTime;
		}
		else {
			minDistance = minDistance - 0 * Health->m_BasicDamage[0].SkillForwardTime;
		}
	
		AttackDistance_J = minDistance;
	}
	else
	{
		AttackDistance_J = 1200;
	}

	FVector Start;
	FVector End;

	Start = ReleaseDistance(AttackDistance_J)[0];
	End = ReleaseDistance(AttackDistance_J)[1];
	//处理特效
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Exop_ParticleSystem,
		End, FRotator(0.0f), FVector(1.0f), true, true, ENCPoolMethod::None, true);
	TArray<FHitResult> HitArr = RangedAttackRange(Start, End);
	FHitResult Hit;
	for (int i = 0; i < HitArr.Num(); i++) {
		Hit = HitArr[i];
		m_fDamege = Health->DemageC(m_iAttackType);
		//射线检测的时候应用伤害
		if (Hit.bBlockingHit) {
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, m_fDamege, End, Hit,
				GetOwner()->GetInstigatorController(), GetOwner(), DamageType);

		}
	}
}


void UAttckComponent::Attack()
{
	FVector Start;
	FVector End;
	
	if (GetOwner()) {
		AActor* MyOwner = GetOwner();
		if (m_CurrentAttackActive == AttackMode::RangedAttack) {

			Start = ReleaseDistance(AttackDistance_Y)[0];
			End = ReleaseDistance(AttackDistance_Y)[1];

			//处理特效
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cx_ParticleSystem,
				FVector(0.0f), FRotator(0.0f), FVector(0.0f), true, EPSCPoolMethod::None, true);
			
			FHitResult Hit = RangedAttack(Start, End);
			if (Hit.bBlockingHit) {
				AActor* HitActor = Hit.GetActor();
				m_fDamege = Health->DemageC(m_iAttackType);
				//射线检测的时候应用伤害
				UGameplayStatics::ApplyPointDamage(HitActor, m_fDamege, End, Hit,
					MyOwner->GetInstigatorController(), MyOwner, DamageType);
				
			}
		}
		else if (m_CurrentAttackActive == AttackMode::RangedAttackRange) {
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &UAttckComponent::GetEnemyLocation, 1.5f, false);
			
		}
		
	}
	
}

void UAttckComponent::SphereMultiChannel(ETraceTypeQuery TraceChannel, float Radius)
{
	m_FRadius = Radius;
	m_ETraceChannel = TraceChannel;
}


FHitResult UAttckComponent::RangedAttack(FVector StartLocation,FVector EndLocation)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bTraceComplex = true;
	Params.bReturnFaceIndex = false;
	Params.bReturnPhysicalMaterial = false;
	FHitResult OutHit;
	
	bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation,ECC_Pawn,Params);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 3, 0, 3);

	return OutHit;
}

TArray<FVector> UAttckComponent::ReleaseDistance(float Distance)
{
	AActor* MyOwner;
	TArray<FVector> OutVector;
	FVector StartLocation;
	FVector EndLocation;
	if (GetOwner()) {
		MyOwner = GetOwner();

		StartLocation = MyOwner->GetActorLocation();
		FRotator CurrentRotator = MyOwner->GetActorRotation();
		FVector DirX = MyOwner->GetActorQuat().GetAxisX();
		EndLocation = StartLocation + DirX * Distance;

	}

	OutVector.Add(StartLocation);
	OutVector.Add(EndLocation);

	return OutVector;

}

void UAttckComponent::SetAttackActive(const AttackMode CurrentAttackActive)
{
	m_CurrentAttackActive = CurrentAttackActive;
}

void UAttckComponent::SetAttackSkill(const AttackSkill CurrentAttackSkill)
{
	if (m_CurrentAttackActive == AttackMode::RangedAttack) {
		if (CurrentAttackSkill != AttackSkill::OrdinaryMagic) {
			if (CurrentAttackSkill == AttackSkill::Rifle) {
				m_iAttackType = 1;
			}
			else if(CurrentAttackSkill == AttackSkill::BowArrow)
			{
				m_iAttackType = 2;
			}
			else if (CurrentAttackSkill == AttackSkill::OpeningTheHeavenlyHateWithOneSword)
			{
				m_iAttackType = 3;
			}
		}
	}
	else if(m_CurrentAttackActive == AttackMode::RangedAttackRange)
	{
		if (CurrentAttackSkill == AttackSkill::OrdinaryMagic) {
			m_iAttackType = 0;
		}
	}

}





