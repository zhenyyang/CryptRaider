// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttckComponent.generated.h"

class UDamageType;
class UNiagaraSystem;
class USHealthComponent;


UENUM()
enum class AttackMode : uint8
{
	RangedAttack = 0			UMETA(DisplayName = "RangedAttack"),
	RangedAttackRange = 1		UMETA(DisplayName = "RangedAttackRange")
};

UENUM()
enum class AttackSkill : uint8
{
	Rifle = 1									UMETA(DisplayName = "Rifle"),
	BowArrow = 2								UMETA(DisplayName = "BowArrow"),
	OpeningTheHeavenlyHateWithOneSword = 3		UMETA(DisplayName = "OneSword"),
	OrdinaryMagic = 0							UMETA(DisplayName = "OrdinaryMagic")
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UAttckComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttckComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//数值or射线检测的attack
protected:
	FHitResult RangedAttack(FVector StartLocation, FVector EndLocation);

	TArray<FHitResult> RangedAttackRange(FVector StartLocation,FVector EndLocation);

	void AttackEffect();
	
	void GetEnemyLocation();

public:
	UFUNCTION(BlueprintCallable, Category = "Attack Component")
		void Attack();

	UFUNCTION(BlueprintCallable, Category = "Attack Component")
		void SphereMultiChannel(ETraceTypeQuery TraceChannel, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Attack Component")
		TArray<FVector> ReleaseDistance(float Distance);

	UFUNCTION(BlueprintCallable, Category = "Attack Component")
		void SetAttackActive(const AttackMode CurrentAttackActive);

	UFUNCTION(BlueprintCallable, Category = "Attack Component")
		void SetAttackSkill(const AttackSkill CurrentAttackSkill);


	UPROPERTY(EditAnywhere, Category = "Attack Component")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Component")
		float AttackDistance_Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Component")
		float AttackDistance_J;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Component")
		UNiagaraSystem* Exop_ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Component")
		UParticleSystem* Cx_ParticleSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Component")
		TSubclassOf<UActorComponent> ComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Component")
		TArray<FName> Tags;
	

private:

	float m_FRadius;

	ETraceTypeQuery m_ETraceChannel;

	AttackMode m_CurrentAttackActive;

	AttackSkill m_CurrentAttackSkill;

	float m_fDamege;

	int32 m_iAttackType;

	USHealthComponent* Health;

	FTimerHandle Handle;
};
