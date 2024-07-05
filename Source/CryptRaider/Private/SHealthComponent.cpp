// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DefaultsHealth = 100;
	DefaultsDamege = 20;

	m_BasicDamage.Add(FST_BasicDamage());

	SetIsReplicatedByDefault(true);

}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() != ROLE_Authority) {
		return;
	}
	
	AActor* MyOwner = GetOwner();
	if (MyOwner) {
		//绑定伤害事件
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
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

	UE_LOG(LogTemp, Log, TEXT("TakeAnyDamage change : ") , Health);
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

	UDataTable* DataTableInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DevelopmentContact/Tool/DataTable/ST_BaseDamege.ST_BaseDamege'"));
	if (!DataTableInfo) {
		return 0;
	}
	for (FName RowName : DataTableInfo->GetRowNames()) {
		FST_BasicDamage* BD = DataTableInfo->FindRow<FST_BasicDamage>(RowName,"AttackType");
		if (BD->AttackType == AttackType) {
			DamegeLevel = BD->DamegeLevel;
			bContinuousDamage = BD->bContinuousDamage;
			ContinuousDamageTime = BD->ContinuousDamageTime;
			ContinuousDamageRatio = BD->ContinuousDamageRatio;
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

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//同步数据到所有客户端和服务器
	DOREPLIFETIME(USHealthComponent,Health);
}

