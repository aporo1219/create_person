// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorFloating.generated.h"

UCLASS()
class PROJECTCPP_API AActorFloating : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorFloating();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> VisualMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
