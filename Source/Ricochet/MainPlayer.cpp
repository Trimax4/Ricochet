// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "Fireball.h"
#include "Engine.h"
#include <EngineGlobals.h>

float RunModifier = 1400;

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

AMainPlayer::AMainPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//air control is float allows direction change while jumping
	GetCharacterMovement()->AirControl = 1;

	// Create a CameraComponent 
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachTo(GetCapsuleComponent());

	// Position the camera a bit above the eyes
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
	// Allow the pawn to control rotation.
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);         // only the owning player will see this mesh
	FirstPersonMesh->AttachTo(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	GetMesh()->SetOwnerNoSee(true);

}

/////////////////////////////
////////PROTECTED BELOW
/////////////////////////////
// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("We are using FPSCharacter!"));
	}
	
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	InputComponent->BindAxis("MoveZ", this, &AMainPlayer::MoveZ);
	InputComponent->BindAxis("MoveX", this, &AMainPlayer::MoveX);
	

	InputComponent->BindAxis("LookX", this, &AMainPlayer::AddControllerYawInput);
	InputComponent->BindAxis("LookY", this, &AMainPlayer::AddControllerPitchInput);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayer::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AMainPlayer::OnStopJump);
	//InputComponent->BindAction("LeftShift", IE_Pressed, this, &AMainPlayer::SprintStart);
	//InputComponent->BindAction("LeftShift", IE_Released, this, &AMainPlayer::SprintStop);
	InputComponent->BindAction("LeftShift", IE_Pressed, this, &AMainPlayer::Dash);
	InputComponent->BindAction("LeftShift", IE_Released, this, &AMainPlayer::DashStop);

	PlayerInputComponent->BindAction("LeftClick", IE_Pressed, this, &AMainPlayer::LeftClick);
}

void AMainPlayer::MoveZ(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		if (DashFlag)
		{
			if (Value > 0)
			{
				const FVector ForwardDir = this->GetRootComponent()->GetForwardVector();
				this->GetActorLocation() += ForwardDir * DodgeWeight;
			}
			else
			{
				const FVector BackwardsDir = this->GetRootComponent()->GetForwardVector();
				this->GetActorLocation() += BackwardsDir * DodgeWeight;
				//GetCharacterMovement()->Velocity += BackwardsDir * -DodgeWeight;
			}
			DashFlag = false;
		}
		
		
	}
}

//controls X axis and implements the dash mechanic
void AMainPlayer::MoveX(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{	
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		if (DashFlag)
		{
			if (Value > 0)
			{
				const FVector RightDir = this->GetRootComponent()->GetRightVector();
				GetCharacterMovement()->Velocity += RightDir * DodgeWeight;
			}
			else
			{
				const FVector LeftDir = this->GetRootComponent()->GetRightVector();
				GetCharacterMovement()->Velocity += LeftDir * -DodgeWeight;
			}	
			DashFlag = false;	
		}
		
	}
}

void AMainPlayer::Dash()
{
	DashFlag = true;
	FVector XVector = this->GetRootComponent()->GetRightVector();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(10, 5.f, FColor::Yellow, FString::Printf(TEXT("Some variable values: x: %f, y: %f, z: %f"), XVector.X, XVector.Y, XVector.Z));
	}
}

void AMainPlayer::DashStop()
{
	DashFlag = false;
}

void AMainPlayer::OnStartJump()
{
	bPressedJump = true;			
}
void AMainPlayer::OnStopJump()
{
	bPressedJump = false;
}



void AMainPlayer::LeftClick()
{
	
	// Attempt to fire a projectile.
		if (ProjectileClass != NULL)
		{
			
			// Get the camera transform.
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			// Transform MuzzleOffset from camera space to world space.
			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = CameraRotation;
			// Skew the aim to be slightly upwards.
			MuzzleRotation.Pitch += 10.0f;
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				
				// Spawn the projectile at the muzzle.
				AFireball* const Projectile = World->SpawnActor<AFireball>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					// Set the projectile's initial trajectory.
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
				
			}
		}
}

void AMainPlayer::SprintStart()
{
	GetCharacterMovement()->MaxWalkSpeed += RunModifier;
}

void AMainPlayer::SprintStop()
{
	GetCharacterMovement()->MaxWalkSpeed -= RunModifier;
}