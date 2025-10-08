// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPawn.h"
#include "GoalActor.generated.h"

UCLASS()
class MYPROJECT_API AGoalActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoalActor();

	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		USceneComponent* mRoot;//ルートコンポーネント
	UPROPERTY(Category = "C_CODE", VisibleAnywhere)
		UStaticMeshComponent* mStaticMesh;//静的メッシュコンポーネント

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	AMyPawn* p;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OverlapBegin(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);


};
