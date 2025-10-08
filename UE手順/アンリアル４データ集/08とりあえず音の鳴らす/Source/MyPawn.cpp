// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//ルートコンポーネント作成
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//このアクターのRootComponentとして設定


	//オーディオコンポーネント作成
	AudioObj = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioObj"));
	AudioObj->SetupAttachment(RootComponent);

	//サウンドセット
	static ConstructorHelpers::FObjectFinder< USoundBase > find_sound(TEXT("/Game/aaa.aaa"));
	if (find_sound.Succeeded()) {
		Sound_Obj = find_sound.Object;
		AudioObj->SetSound(Sound_Obj);
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	//サウンドスタート
	AudioObj->Play();

	//UGameplayStatics::PlaySound2D(GetWorld(), Sound_Obj, 1.0f, 1.0f, 0.0f, nullptr, this);

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

