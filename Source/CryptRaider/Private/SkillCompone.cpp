// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCompone.h"

// Sets default values for this component's properties
USkillCompone::USkillCompone()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	m_BaseSkill.Add(FST_Skill());
	m_WeaponEffect.Add(FST_WeaponEffect());
	// ...
}


// Called when the game starts
void USkillCompone::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void USkillCompone::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USkillCompone::SetBaseSkill(TArray<FST_Skill> BaseSkill)
{
	/*
	UDataTable* DataTableInfo = LoadObject<UDataTable>(nullptr,TEXT("/Script/Engine.DataTable'/Game/DevelopmentContact/Tool/DataTable/ST_Skill.ST_Skill'"));
	if (DataTableInfo) {
		FST_Skill* Skill = new FST_Skill();
		for (FST_Skill var : BaseSkill) {
			Skill->AttackType = var.AttackType;
			Skill->ParticleSystem = var.ParticleSystem;
			Skill->SkillForwardTime = var.SkillForwardTime;
			DataTableInfo->AddRow("");
		}

	}*/
	m_BaseSkill = BaseSkill;
}

FST_Skill& USkillCompone::GetBaseSkillByAttackType(int32 AttackType)
{
	UDataTable* DataTableInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DevelopmentContact/Tool/DataTable/ST_Skill.ST_Skill'"));
	if (!DataTableInfo) {
		return m_BaseSkill[0];
	}
	for (FName RowName : DataTableInfo->GetRowNames()) {
		FST_Skill* UserInfo = DataTableInfo->FindRow<FST_Skill>(RowName,"AttackType");
		
		if (UserInfo->AttackType == AttackType) {
			return *UserInfo;
		}
	}
	/*
	for (int i = 0; i < m_BaseSkill.Num(); i++) {
		if (m_BaseSkill[i].AttackType == AttackType) {
			return m_BaseSkill[i];
			//return m_BaseSkill.Pop(true);
		}
	}*/

	return m_BaseSkill[0];
}

TArray<FST_Skill> USkillCompone::GetAllSkill()
{
	TArray<FST_Skill> Skill_Temp{FST_Skill()};

	UDataTable* DataTableInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DevelopmentContact/Tool/DataTable/ST_Skill.ST_Skill'"));
	if (!DataTableInfo) {
		return Skill_Temp;
	}
	for (FName RowName : DataTableInfo->GetRowNames()) {
		FST_Skill* UserInfo = DataTableInfo->FindRow<FST_Skill>(RowName, "AttackType");
		if (UserInfo) {
			Skill_Temp.Add(*UserInfo);
		}
	}

	return Skill_Temp;
}

void USkillCompone::SetWeaponEffect(TArray<FST_WeaponEffect> WeaponEffect)
{
	m_WeaponEffect = WeaponEffect;
}

FST_WeaponEffect& USkillCompone::GetWeaponEffectByAttackType(int32 AttackType)
{
	UDataTable* DataTableInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DevelopmentContact/Tool/DataTable/ST_WeaponEffect.ST_WeaponEffect'"));
	if (!DataTableInfo) {
		return m_WeaponEffect[0];
	}
	for (FName RowName : DataTableInfo->GetRowNames()) {
		FST_WeaponEffect* UserInfo = DataTableInfo->FindRow<FST_WeaponEffect>(RowName, "AttackType");
		if (UserInfo->AttackType == AttackType) {
			return *UserInfo;
		}
	}
	/*
	if (m_WeaponEffect.IsEmpty()) {
		return m_WeaponEffect[0];
	}
	for (int i = 0; i < m_WeaponEffect.Num(); i++) {
		if (m_WeaponEffect[i].AttackType == (AttackType+1)) {
			return m_WeaponEffect[i];
		}
	}*/

	return m_WeaponEffect[0];
}

TArray<FST_WeaponEffect> USkillCompone::GetAllWeaponEffect()
{
	TArray<FST_WeaponEffect> WeaponEffect_Temp{FST_WeaponEffect()};

	UDataTable* DataTableInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/DevelopmentContact/Tool/DataTable/ST_Skill.ST_Skill'"));
	if (!DataTableInfo) {
		return WeaponEffect_Temp;
	}
	for (FName RowName : DataTableInfo->GetRowNames()) {
		FST_WeaponEffect* UserInfo = DataTableInfo->FindRow<FST_WeaponEffect>(RowName, "AttackType");
		if (UserInfo) {
			WeaponEffect_Temp.Add(*UserInfo);
		}
	}

	return WeaponEffect_Temp;

}

