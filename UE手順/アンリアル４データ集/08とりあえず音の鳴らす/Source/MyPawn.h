// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"


UCLASS()
class MYPROJECT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USoundBase* Sound_Obj;

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRootCollider;	//ルートコンポーネント（必須）
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UAudioComponent* AudioObj;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
