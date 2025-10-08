// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"//PrintString
#include "MyProjectGameModeBase.h"
#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//USceneComponent* mRoot;//ルートコンポーネント
	mRoot = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Cast<USceneComponent>(mRoot);//このアクターのRootComponentとして設定

	//USkeletalMeshComponent* mSMesh;//スケルタルメッシュコンポーネント
	mSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SMesh"));
	mSMesh->SetupAttachment(mRoot);//RootComponentの子にする
	//スケルタルメッシュ登録
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> VisualAsset(TEXT("/Game/Blade_ChickenBlade/SK_Blade_ChickenBlade.SK_Blade_ChickenBlade"));
	mSMesh->SetSkeletalMesh(VisualAsset.Object);//登録

	mSMesh->SetSimulatePhysics(true);	   //物理ON
	mSMesh->SetGenerateOverlapEvents(true);//オーバーラップイベントをtrueにする(GenerateOverlapEventsフラグON)
	mSMesh->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OverlapBegin);//オーバーラップスタート時イベント


	//コリジョンセット カスタム設定
	mSMesh->BodyInstance.SetCollisionProfileName("Custom");
	mSMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);//オブジェクトタイプを動いてるモノにする
	mSMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//コリジョンレスポンス
	mSMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);//全てオールブロック設定
	mSMesh->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);//のちPawnをオーバーラップ設定

	
	


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
void AItemActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyProjectGameModeBase* GameMode = Cast<AMyProjectGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->mPoint += 100;
		Destroy();//アクター削除
	}

}

