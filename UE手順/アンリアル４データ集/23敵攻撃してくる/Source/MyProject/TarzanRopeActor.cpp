// Fill out your copyright notice in the Description page of Project Settings.


#include "TarzanRopeActor.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"

// Sets default values
ATarzanRopeActor::ATarzanRopeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // ケーブルコンポーネントを作成し、ルートコンポーネントに設定
    mCable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
    RootComponent = mCable;


    // ケーブルの末端用シーンコンポーネントを作成
    mEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
    mEndPoint->SetWorldLocation(FVector(0,0,0));


    // ケーブルの設定
    //ケーブルのエンド名を設定をEndSocketに設定、ケーブルエンドはmEndPointで動かす
    mCable->SetAttachEndToComponent(mEndPoint, FName("EndSocket"));
    mCable->CableLength = 300.0f; // ケーブルの長さを設定
    mCable->bAttachStart = true;  //ケーブルの先端をアタッチして、位置を固定
    mCable->bAttachEnd   = false;   
    mCable->bEnableCollision = false;//ケーブルのセグメントに衝突判定をOFF
  

    //モデルアセット登録
    static ConstructorHelpers::FObjectFinder<UStaticMesh> VisualAsset(TEXT("/Engine/BasicShapes/Cube"));
    //スタティックメッシュコンポーネント作成
    mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    mMesh->SetupAttachment(mCable);
    mMesh->SetStaticMesh(VisualAsset.Object);//登録
    // 立方体設定
    mMesh->SetRelativeScale3D(FVector(0.5)); // サイズを調整
    mMesh->SetRelativeLocation(FVector(0, 0, 0)); // 位置を調整
    mMesh->BodyInstance.SetCollisionProfileName("OverlapAll");//コリジョンセット
    mMesh->SetSimulatePhysics(false);  // 物理シミュレーションを無効にする
    mMesh->SetNotifyRigidBodyCollision(false);
    mMesh->SetGenerateOverlapEvents(true);
    mMesh->OnComponentBeginOverlap.AddDynamic(this, &ATarzanRopeActor::OverlapBegin);
    mMesh->OnComponentEndOverlap.AddDynamic(this, &ATarzanRopeActor::OverlapEnd);
}

// Called when the game starts or when spawned
void ATarzanRopeActor::BeginPlay()
{
	Super::BeginPlay();

    m_hit = false;
}

// Called every frame
void ATarzanRopeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //ケーブルの線（パーティクル）の位置を取得
    TArray<FVector> Locations;
    mCable->GetCableParticleLocations(Locations);
 
  
    //先頭・末端獲得
    FVector  end_pos   = Locations[Locations.Num() - 1];//末端
    FVector  endf_pos  = Locations[Locations.Num() - 2];//末端の前
    FVector  start_pos = Locations[0];                  //先頭
    mMesh->SetWorldLocation(end_pos);//mEndPointを先頭位置に設定


    //末端と先頭の位置から向きベクトルを取得
    FQuat mNowQ;
    FVector w(0, 0, 1);
    FVector vec_cable = endf_pos - end_pos;
    vec_cable.Normalize();
    //向きベクトルからクォータニオンを作成
    float r = FVector::DotProduct(w, vec_cable);		//wベクトルを0度として回転角度
    FVector rvec = FVector::CrossProduct(w, vec_cable);	//wベクトルと移動方向から回転軸ベクトル求める
    rvec.Normalize();//回転軸ベクトルを正規化
    //法線の場合、不可ベクトルが発生する場合がある
    if (rvec.IsNearlyZero())
        rvec = FVector(0, 0, 1);
    //回転角度と回転軸ベクトルからクォータニオンを取得
    mNowQ = FQuat(rvec, acos(r));

    //mEndPointを向き方向に回転
    mMesh->SetWorldRotation(mNowQ);

    if (m_hit == true)
    {
        //キャラクターにケーブルの末端を持たせる
        //ワールド内のアクターを探す
        UWorld* const World = GetWorld();
        TArray<AActor*> a;
        UGameplayStatics::GetAllActorsOfClass(World, AMyPawn::StaticClass(), a);
        if (a[0] != nullptr)
        {
            AMyPawn* p = Cast<AMyPawn>(a[0]);

            //キャラクター強制移動
            p->mCapsule->SetWorldLocation(end_pos);
            p->mSMesh->SetWorldRotation(mNowQ);

            //ロープからの解除
            if (p->go == true)
            {
                m_hit = false;
                p->mCapsule->SetSimulatePhysics(true); 
                p->mCapsule->SetNotifyRigidBodyCollision(true);//コリジョン有効
                p->mCapsule->RecreatePhysicsState();           //物理状態を再作成
            }
        }
    }
   
}


//キャラクターとオーバーラップ（着いた時）
void ATarzanRopeActor::OverlapBegin(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
   

    AMyPawn* p = Cast<AMyPawn>(&OtherActor[0]);
    if (p != nullptr)
    {
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("hit_in")), true, true, FColor::Cyan, 2.f);
        m_hit = true;


        p->go = false;
        //ケーブルに移動方向の力を加える
        FVector vec = p->mMove->Velocity;
        mCable->CableForce.Set(vec.X, vec.Y, vec.Z);
        mCable->CableForce.Normalize();
        mCable->CableForce.X *= 3000;
        mCable->CableForce.Y *= 3000;
        mCable->CableForce.Z *= 3000;
       
        //遅延処理
        UWorld* World = GetWorld();
        FTimerHandle _TimerHandle;
        World->GetTimerManager().SetTimer(_TimerHandle, [&]()
            {
                mCable->CableForce.Set(0, 0, 0);//少し経ってから、力を0にして自由にする
            },
            0.5f, false);

        p->mCapsule->SetSimulatePhysics(false);
        p->mCapsule->SetNotifyRigidBodyCollision(false);//コリジョン無効
        p->mCapsule->RecreatePhysicsState();            //物理状態を再作成
    }

  
}

//キャラクターとオーバーラップ（離れた時）
void ATarzanRopeActor::OverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("hit_out")), true, true, FColor::Cyan, 2.f);
    m_hit = false;

    AMyPawn* p = Cast<AMyPawn>(&OtherActor[0]);
    if (p != nullptr)
    {
        p->mCapsule->SetSimulatePhysics(true);
        p->mCapsule->SetNotifyRigidBodyCollision(true);//コリジョン有効
        p->mCapsule->RecreatePhysicsState();           //物理状態を再作成
    }
}

