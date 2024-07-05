// Fill out your copyright notice in the Description page of Project Settings.


#include "AttckComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "SHealthComponent.h"
#include "SkillCompone.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UAttckComponent::UAttckComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_CurrentAttackActive = AttackMode::LineAttack;
	m_ETraceChannel = ETraceTypeQuery::TraceTypeQuery1;
	m_CurrentAttackSkill = AttackSkill::Rifle;

	m_FRadius = 150;
	AttackDistance_Y = 3000;
	AttackDistance_J = 1200;
	m_iAttackType = 1;
	ActorsToIgnore.Add(nullptr);

	SetIsReplicated(true);
}


// Called when the game starts
void UAttckComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = Cast<USHealthComponent>(GetOwner()->GetComponentByClass(HealthComp));
	Skill = Cast<USkillCompone>(GetOwner()->GetComponentByClass(SkillComp));
}


// Called every frame
void UAttckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FHitResult> UAttckComponent::SphereAttackTrace(FVector StartLocation, FVector EndLocation)
{
	TArray<FHitResult> HitArray;
	TArray<FHitResult> OutHits;
	
	//FString HitTagEnemy;
	//FString HitTagEnemyFilter;

	bool HitActor = UKismetSystemLibrary::SphereTraceMulti(GetWorld(), EndLocation, EndLocation, m_FRadius,
		m_ETraceChannel, false, ActorsToIgnore, EDrawDebugTrace::Persistent,
		HitArray, true, FLinearColor::Red, FLinearColor::Green, 5);
	
	if (HitActor) {
		ExcludeRedundantObjects(HitArray, OutHits);
	}
	return OutHits;

}

TArray<FHitResult> UAttckComponent::BoxAttackTrace(FVector StartLocation, FVector EndLocation, FVector Range)
{
	TArray<FHitResult> HitArray;
	TArray<FHitResult> OutHits;

	bool HitActor = UKismetSystemLibrary::BoxTraceMulti(GetWorld(), StartLocation, EndLocation, Range,FRotator(0.f),
		m_ETraceChannel, false, ActorsToIgnore, EDrawDebugTrace::Persistent,
		HitArray, true, FLinearColor::Red, FLinearColor::Green, 5);
	if (HitActor) {
		ExcludeRedundantObjects(HitArray, OutHits);
	}

	return OutHits;
}

void UAttckComponent::LineAttack()
{

}

void UAttckComponent::SphereAttack()
{
	FVector Start;
	FVector End;

	Start = ReleaseDistance(AttackDistance_J)[0];
	End = ReleaseDistance(AttackDistance_J)[1];
	//处理特效
	Exop_ParticleSystem = Skill->GetBaseSkillByAttackType(m_iAttackType).ParticleSystem;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Exop_ParticleSystem,
		End, FRotator(0.0f), FVector(1.0f), true, true, ENCPoolMethod::None, true);
	TArray<FHitResult> HitArr = SphereAttackTrace(Start, End);
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

void UAttckComponent::BoxAttack()
{
	FVector Start = ReleaseDistance(AttackDistance_Y)[0];
	FVector End = ReleaseDistance(AttackDistance_Y)[1];

	Exop_ParticleSystem = Skill->GetBaseSkillByAttackType(m_iAttackType).ParticleSystem;
	TArray<FHitResult> HitArr = BoxAttackTrace(Start, End, m_Range);

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
			minDistance = minDistance - 300 * Skill->m_BaseSkill[0].SkillForwardTime;
		}
		else {
			minDistance = minDistance - 0 * Skill->m_BaseSkill[0].SkillForwardTime;
		}
	
		AttackDistance_J = minDistance;
	}
	else
	{
		AttackDistance_J = 1200;
	}

	SphereAttack();

}

void UAttckComponent::ExcludeRedundantObjects(TArray<FHitResult> HitArray, TArray<FHitResult>& OutHits)
{
	int32 len_tag1 = HitArray.Num();
	for (int i = 0; i < len_tag1 - 1; i++) {
		int tempIndex = i + 1;
		for (int j = i + 1; j < len_tag1; j++) {
			if (HitArray[i].GetActor()->GetFName() == HitArray[j].GetActor()->GetFName()) {
				FHitResult temp = HitArray[j];
				HitArray[j] = HitArray[tempIndex];
				HitArray[tempIndex] = temp;
				tempIndex++;
			}
		}
	}

	//OutHits.Add(HitArray[0]);
	for (int i = 0; i < len_tag1; i++) {
		//FHitResult Hit = HitArray[i];
		//bool Tag_A = HitArray[i].GetActor()->Tags.Contains(Tags[0]);
		//bool Tag_B = HitArray[i].GetActor()->Tags.Contains(Tags[1]);
		for (int j = i + 1; j < len_tag1; j++) {
			if (HitArray[i].GetActor()->GetFName() != HitArray[j].GetActor()->GetFName()) {
				OutHits.Add(HitArray[i]);
				i = j;
				continue;
			}
		}
		if (i == len_tag1 - 1) {
			OutHits.Add(HitArray[i]);
		}
	}

	/*int32 len_tag2 = OutHits.Num();
	for (int i = len_tag2 -1 ; i > 0;i--) {
			if (OutHits[i].GetActor() == OutHits[i].GetActor()) {
				if (OutHits[i].GetActor()->GetFName() == OutHits[i].GetActor()->GetFName()) {
					OutHits.RemoveAt(i);
					i = OutHits.Num() - 1;
					j = i - 1;
				}
			}
		}
	}*/
}

void UAttckComponent::ServerAttack_Implementation()
{
	Attack();
}

bool UAttckComponent::ServerAttack_Validate() {
	return true;
}

void UAttckComponent::Attack()
{
	if (GetOwnerRole() < ROLE_Authority) {
		ServerAttack();
	}
	FVector Start;
	FVector End;
	
	if (GetOwner()) {
		AActor* MyOwner = GetOwner();
		if (m_CurrentAttackActive == AttackMode::LineAttack) {

			Start = ReleaseDistance(AttackDistance_Y)[0];
			End = ReleaseDistance(AttackDistance_Y)[1];

			//处理特效
			Cx_ParticleSystem = Skill->GetWeaponEffectByAttackType(m_iAttackType).LaunchPointParticle;
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Cx_ParticleSystem,
				FVector(0.0f), FRotator(0.0f), FVector(0.0f), true, EPSCPoolMethod::None, true);
			 //生成点特效处理
			
			FHitResult Hit = LineAttackTrace(Start, End);
			if (Hit.bBlockingHit) {
				AActor* HitActor = Hit.GetActor();
				m_fDamege = Health->DemageC(m_iAttackType);
				//射线检测的时候应用伤害
				UGameplayStatics::ApplyPointDamage(HitActor, m_fDamege, End, Hit,
					MyOwner->GetInstigatorController(), MyOwner, DamageType);
				
			}
		}
		else if (m_CurrentAttackActive == AttackMode::SphereAttack) {
			
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &UAttckComponent::GetEnemyLocation, Skill->GetBaseSkillByAttackType(m_iAttackType).SkillForwardTime, false);
			
		}
		else if (m_CurrentAttackActive == AttackMode::BoxAttack) {
			GetWorld()->GetTimerManager().SetTimer(Handle, this, &UAttckComponent::BoxAttack, Skill->GetBaseSkillByAttackType(m_iAttackType).SkillForwardTime, false);
		}
		
	}
	
}

void UAttckComponent::SphereMultiChannel(ETraceTypeQuery TraceChannel, float Radius)
{
	m_FRadius = Radius;
	m_ETraceChannel = TraceChannel;
}


FHitResult UAttckComponent::LineAttackTrace(FVector StartLocation,FVector EndLocation)
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
	if (m_CurrentAttackActive == AttackMode::LineAttack) {
		if (CurrentAttackSkill == AttackSkill::Rifle) {
			m_iAttackType = 1;
		}
		else if (CurrentAttackSkill == AttackSkill::BowArrow)
		{
			m_iAttackType = 2;
		}
	}
	else if(m_CurrentAttackActive == AttackMode::SphereAttack)
	{
		if (CurrentAttackSkill == AttackSkill::OrdinaryMagic) {
			m_iAttackType = 0;
		}
	}
	else if(m_CurrentAttackActive == AttackMode::BoxAttack)
	{
		if (CurrentAttackSkill == AttackSkill::OpeningTheHeavenlyHateWithOneSword)
		{
			m_iAttackType = 3;
		}
	}
}





