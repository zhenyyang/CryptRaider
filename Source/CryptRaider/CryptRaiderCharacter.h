// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CryptRaiderCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class USpringArmComponent;

UCLASS()
class ACryptRaiderCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpringArm", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	

public:
		
	ACryptRaiderCharacter();

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class USHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class UAttckComponent* AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		class USkillCompone* SkillComp;

	UFUNCTION(BlueprintCallable, Category = Weapon)
		void Attack_Exe();

	UFUNCTION()
		void OnHealthChanged(class USHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamegeCause);

	UPROPERTY(Replicated,EditAnywhere, BlueprintReadWrite, Category = Input)
		bool bDie;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	virtual void BeginPlay();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	void SetAttackCharactor();

	

private:
	FTimerHandle Handle;

};

