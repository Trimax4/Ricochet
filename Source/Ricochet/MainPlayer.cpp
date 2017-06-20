// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "Engine.h"

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
	}
}

void AMainPlayer::MoveX(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMainPlayer::OnStartJump()
{
	bPressedJump = true;
}

void AMainPlayer::OnStopJump()
{
	bPressedJump = false;
}