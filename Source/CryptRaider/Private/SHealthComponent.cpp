// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultsHealth = 100;
	DefaultsDamege = 20;

	m_BasicDamage.Add(FST_BasicDamage());
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()) {
		//绑定伤害事件
		GetOwner()->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}
	Health = DefaultsHealth;
}


// Called every frame
void USHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamageActor, float Damege, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamegeCause)
{
	if (Damege <= 0) {
		return;
	}
	Health = FMath::Clamp(Health - Damege, 0.0f , DefaultsHealth);
	//广播伤害事件
	OnHealthChanged.Broadcast(this, Health, Damege, DamageType, InstigatedBy, DamegeCause);

	UE_LOG(LogTemp, Log, TEXT("TakeAnyDamage change : "),Health);
}

void USHealthComponent::SetFSTBasicDamage(TArray<FST_BasicDamage> BasicDamageValue)
{
	m_BasicDamage = BasicDamageValue;
}

float USHealthComponent::DemageC(int32 AttackType)
{
	int32 DamegeLevel = 0;
	bool bContinuousDamage = false;
	float ContinuousDamageTime = 0.0f;
	float ContinuousDamageRatio = 0.0f;

	for (const FST_BasicDamage& var : m_BasicDamage) {
		if (var.AttackType == AttackType) {
			DamegeLevel = var.DamegeLevel;
			bContinuousDamage = var.bContinuousDamage;
			ContinuousDamageTime = var.ContinuousDamageTime;
			ContinuousDamageRatio = var.ContinuousDamageRatio;
		}
	}

	float CouterDamege = DamegeLevel;
	if (bContinuousDamage) {
		for (int i = 0; i < ContinuousDamageTime; i++) {
			CouterDamege += ContinuousDamageRatio * DamegeLevel;
		}
	}

	return CouterDamege;

}

