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
	m_BaseSkill = BaseSkill;
}

FST_Skill& USkillCompone::GetBaseSkillByAttackType(int32 AttackType)
{
	if (m_BaseSkill.IsEmpty()) {
		return m_BaseSkill[0];
	}
	for (int i = 0; i < m_BaseSkill.Num(); i++) {
		if (m_BaseSkill[i].AttackType == AttackType) {
			return m_BaseSkill[i];
			//return m_BaseSkill.Pop(true);
		}
	}
	return m_BaseSkill[0];
}

TArray<FST_Skill> USkillCompone::GetAllSkill()
{
	return m_BaseSkill;
}

void USkillCompone::SetWeaponEffect(TArray<FST_WeaponEffect> WeaponEffect)
{
	m_WeaponEffect = WeaponEffect;
}

FST_WeaponEffect& USkillCompone::GetWeaponEffectByAttackType(int32 AttackType)
{
	if (m_WeaponEffect.IsEmpty()) {
		return m_WeaponEffect[0];
	}
	for (int i = 0; i < m_WeaponEffect.Num(); i++) {
		if (m_WeaponEffect[i].AttackType == (AttackType+1)) {
			return m_WeaponEffect[i];
		}
	}
	return m_WeaponEffect[0];
}

TArray<FST_WeaponEffect> USkillCompone::GetAllWeaponEffect()
{
	return m_WeaponEffect;
}

