// Copyright Epic Games, Inc. All Rights Reserved.

#include "CryptRaiderCharacter.h"
#include "CryptRaiderProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AttckComponent.h"
#include "SHealthComponent.h"
#include "SkillCompone.h"
#include "Net/UnrealNetwork.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"



//////////////////////////////////////////////////////////////////////////
// ACryptRaiderCharacter

ACryptRaiderCharacter::ACryptRaiderCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	bDie = false;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Healthcomp"));
	AttackComp = CreateDefaultSubobject<UAttckComponent>(TEXT("Attackcomp"));
	SkillComp = CreateDefaultSubobject<USkillCompone>(TEXT("Skillcomp"));

}

void ACryptRaiderCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//绑定一个事件分发器
	HealthComp->OnHealthChanged.AddDynamic(this, &ACryptRaiderCharacter::OnHealthChanged);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ACryptRaiderCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACryptRaiderCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACryptRaiderCharacter::Look);


	}
}

void ACryptRaiderCharacter::SetAttackCharactor()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->bUseControllerRotationYaw = false;
}


void ACryptRaiderCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//同步数据到所有客户端和服务器
	DOREPLIFETIME(ACryptRaiderCharacter, bDie);

}

void ACryptRaiderCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACryptRaiderCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACryptRaiderCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ACryptRaiderCharacter::GetHasRifle()
{
	return bHasRifle;
}

void ACryptRaiderCharacter::Attack_Exe()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->bUseControllerRotationYaw = true;
	AttackComp->Attack();
	GetWorld()->GetTimerManager().SetTimer(Handle,this, &ACryptRaiderCharacter::SetAttackCharactor,1.5f,false);
}

void ACryptRaiderCharacter::OnHealthChanged(USHealthComponent* OwnerHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamegeCause)
{
	if (Health <= 0 && !bDie) {
		bDie = true;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->StopMovementImmediately();
		DetachFromControllerPendingDestroy();
		SetLifeSpan(2.0f);
	}
}
