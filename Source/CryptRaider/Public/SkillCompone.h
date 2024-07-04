// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "SkillCompone.generated.h"

class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FST_Skill : public FTableRowBase {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillYuanc")
		int32 AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillYuanc")
		UNiagaraSystem* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillYuanc")
		float SkillForwardTime;
};

USTRUCT(BlueprintType)
struct FST_WeaponEffect : public FTableRowBase {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponEffect")
		int32 AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponEffect")
		UParticleSystem* LaunchPointParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponEffect")
		UParticleSystem* ContactPointParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponEffect")
		float SkillForwardTime;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API USkillCompone : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillCompone();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "SkillComponent")
		void SetBaseSkill(TArray<FST_Skill> BaseSkill);

	UFUNCTION(BlueprintCallable, Category = "SkillComponent")
		FST_Skill& GetBaseSkillByAttackType(int32 AttackType);

	UFUNCTION(BlueprintCallable, Category = "SkillComponent")
		TArray<FST_Skill> GetAllSkill();

	UFUNCTION(BlueprintCallable, Category = "SkillComponent")
		void SetWeaponEffect(TArray<FST_WeaponEffect> BaseSkill);

	UFUNCTION(BlueprintCallable, Category = "SkillComponent")
		FST_WeaponEffect& GetWeaponEffectByAttackType(int32 AttackType);

	UFUNCTION(BlueprintCallable, Category = "SkillComponent")
		TArray<FST_WeaponEffect> GetAllWeaponEffect();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		TArray<FST_Skill> m_BaseSkill;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		TArray<FST_WeaponEffect> m_WeaponEffect;

};
