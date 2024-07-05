// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "SHealthComponent.generated.h"

class UDamageType;
class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignatrue, USHealthComponent*
	, Healthcomp, float, Damege, float, DamegeDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

USTRUCT(BlueprintType)
struct FST_BasicDamage : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int32 AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		int32 DamegeLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		bool bContinuousDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float ContinuousDamageTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float ContinuousDamageRatio;

};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
		TArray<FST_BasicDamage> m_BasicDamage;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		void SetFSTBasicDamage(TArray<FST_BasicDamage> BasicDamageValue);

	float DemageC(int32 AttackType);

	//创建事件分发器；
	UPROPERTY(BlueprintAssignable, Category = "HealthComponent")
		FOnHealthChangedSignatrue OnHealthChanged;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite , Category = "HealthComponent")
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultsHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,  Category = "HealthComponent")
		float DefaultsDamege;

	UFUNCTION(BlueprintCallable,Category = "HealthComponent")
		void HandleTakeAnyDamage(AActor* DamageActor, float Damege,const UDamageType* DamageType,AController *InstigatedBy,AActor* DamegeCause);

	
	

};
