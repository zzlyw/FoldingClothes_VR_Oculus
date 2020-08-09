// Fill out your copyright notice in the Description page of Project Settings.

#include "ImageCaptureActor.h"
#include "ClothBall.h"
#include <string>



FReceiveThread *FR;

// Sets default values
AImageCaptureActor::AImageCaptureActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RunningCount = 0;
}

AImageCaptureActor::~AImageCaptureActor()
{


	bool iskill = ThreadEnd();
}



// Called when the game starts or when spawned
void AImageCaptureActor::BeginPlay()
{
	Super::BeginPlay();

	//for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	AStaticMeshActor *mesh = *ActorItr;
	//	UE_LOG(LogTemp, Warning, TEXT("name: %s"), *(ActorItr->GetName()));
	//}



	//FString Ffilename = FPaths::ProjectSavedDir()+"imageshots_"+ FDateTime::Now().ToString();
	//FString Fmode = "wb";
	//std::string filename = TCHAR_TO_UTF8(*Ffilename);
	//std::string mode = TCHAR_TO_UTF8(*Fmode);
	//ImageFile = fopen(filename.c_str(), mode.c_str());


}

void AImageCaptureActor::ConnectServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("sender"), false);

	FIPv4Address ip(127,0,0,1);
	ip = FIPv4Address(IP0, IP1, IP2, IP3);

	UE_LOG(LogTemp, Warning, TEXT("Connecting to server: %d.%d.%d.%d "), IP0, IP1, IP2, IP3);
	


	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(4399);

	Connected = Socket->Connect(*addr);



	FR = new FReceiveThread(Socket);
	m_RecvThread = FRunnableThread::Create(FR, TEXT("RecvThread"));



	UGameViewportClient* gameViewport = GEngine->GameViewport;
	InViewport = gameViewport->Viewport;
}

// Called every frame
void AImageCaptureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("MyCharacter's Bool is %s"), (FR->data_received  ? TEXT("true") : TEXT("false")));



	if (FR != NULL && FR->data_received)
	{
		axisPoint1_x = FR->s_int[0];
		axisPoint1_y = FR->s_int[1];
		axisPoint2_x = FR->s_int[2];
		axisPoint2_y = FR->s_int[3];
		fingerPoint1_x = FR->s_int[4];
		fingerPoint1_y = FR->s_int[5];
		fingerPoint2_x = FR->s_int[6];
		fingerPoint2_y = FR->s_int[7];

		IsNewDataComing = FR->IsNewDataComing; //monitor the new data from external program
	}
}

void AImageCaptureActor::SetIsNewDataComingInReceiveThread()
{
	FR->IsNewDataComing = false;
}

void AImageCaptureActor::TakePictureAndSend()
{
	TArray<FColor> SrcMap;
	TArray<FColor> DestMap;

	FIntRect Rect(0, 0, InViewport->GetSizeXY().X, InViewport->GetSizeXY().Y);
	bool bScreenshotSuccessful = GetViewportScreenShot(InViewport, SrcMap, Rect);

	if (bScreenshotSuccessful)
	{
		FR->IsNewDataComing = false; //send back image and reset the data monitor

		FIntVector Size(InViewport->GetSizeXY().X, InViewport->GetSizeXY().Y, 0);
		FImageUtils::CropAndScaleImage(Size.X, Size.Y, FixWidth, FixHeight, SrcMap, DestMap);

		TArray<uint8> ImageArr;
		for (int i = 0; i < DestMap.Num(); i++)
		{
			//channel color exchange
			uint8 R = DestMap[i].B;
			uint8 G = DestMap[i].G;
			uint8 B = DestMap[i].R;

			ImageArr.Add(R);
			ImageArr.Add(G);
			ImageArr.Add(B);
		}

		UE_LOG(LogTemp, Warning, TEXT("R=%u, G=%u, B=%u"), DestMap[0].R, DestMap[0].G, DestMap[0].B);

		uint8* ImagePtr = ImageArr.GetData();
		if (Connected)
		{
			int32 size = ImageArr.Num();
			UE_LOG(LogTemp, Warning, TEXT("image size: %d"), size);
			int32 sent;
			//uint8* ImageSizeInfo =  (uint8*)size;
			//bool success = Socket->Send(ImageSizeInfo, 4, sent);
			bool success = false;
			//while (!success)
			{
				success = Socket->Send(ImagePtr, size, sent);
			}
			 
		}

		//store to local computer
		TArray<uint8> PNGData;
		FImageUtils::CompressImageArray(FixWidth, FixHeight, DestMap, PNGData);
		myFileName = FPaths::ProjectSavedDir() + "imageshots_" + FDateTime::Now().ToString() + ".png";
		FFileHelper::SaveArrayToFile(PNGData, *myFileName);

	}
}

bool AImageCaptureActor::SocketReceive()
{
	FR = new FReceiveThread(Socket);
	m_RecvThread = FRunnableThread::Create(FR, TEXT("RecvThread"));

	return true;


}



bool AImageCaptureActor::ThreadEnd()
{
	
	if (m_RecvThread != nullptr)
	{
		m_RecvThread->Kill(true);
		delete m_RecvThread;
	}

	
	return true;

}
