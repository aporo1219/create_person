// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "BallPawn.h"
#include "MyProjectGameModeBase.h"
#include "Kismet/KismetSystemLibrary.h"//PrintString

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ルートコンポーネント作成
	mRootCollider = CreateDefaultSubobject<USceneComponent>("RootCollider");
	RootComponent = Cast<USceneComponent>(mRootCollider);//このアクターのRootComponentとして設定

	//スタティックメッシュコンポーネント作成
	mCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//スタティックメッシュ「キューブビジュアル」のアセット登録
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Engine/BasicShapes/Cube"));
	mCubeMesh->SetStaticMesh(CubeVisualAsset.Object);//登録
	mCubeMesh->SetRelativeRotation(FRotator(30, 30, 0));//親からの相対的な回転角度
	mCubeMesh->SetRelativeLocation(FVector(0, 0, 0));	//親からの相対的な位置

	mCubeMesh->BodyInstance.SetCollisionProfileName("OverlapAll");//コリジョンプリセット(OverlapAllを設定)
	mCubeMesh->SetGenerateOverlapEvents(true);//オーバーラップイベントをtrueにする(GenerateOverlapEventsフラグON)
	mCubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OverlapBegin);//オーバーラップスタート時イベント
	

	//回転ムーブコンポーネント作成
	mRotatMove = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatMove"));
	mRotatMove->UpdatedComponent=mCubeMesh;
	mRotatMove->bRotationInLocalSpace = true;		//回転がローカル空間で適用されるか、ワールド空間で適用されるか。
	mRotatMove->PivotTranslation = FVector(0, 0, 0);//回転基準位置
	mRotatMove->RotationRate = FRotator(0, 30, 0);	//回転速度

	
	//-----ルート-----
	mCubeMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//オーバーラップビギンメソッド
void AItemActor::OverlapBegin( UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UKismetSystemLibrary::PrintString(this, "aaa", true, true, FColor::Cyan, 2.f);
	if (Cast<ABallPawn>(OtherActor) != nullptr)
	{
		mRotatMove->RotationRate = FRotator(0, 200, 0);	//回転速度

		//遅延処理(SetTimerを使って1秒後にCollectedを動くようにした)
		UWorld* const World = GetWorld();
		FTimerHandle _TimerHandle;
		World->GetTimerManager().SetTimer(_TimerHandle, this, &AItemActor::Collected, 1.0f, true);
	}
}
void AItemActor::Collected()
{
	//認証してるゲームモードを獲得
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->ItemCollected();//ゲームモード内のメソッドをコール
	}

	Destroy();//アクター削除
}


