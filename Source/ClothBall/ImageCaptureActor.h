// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImageUtils.h"
#include "Networking.h"
#include <stdio.h>
#include "ReceiveThread.h"
#include "ImageCaptureActor.generated.h"

UCLASS(Blueprintable)
class CLOTHBALL_API AImageCaptureActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AImageCaptureActor();
	~AImageCaptureActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	//int RunningCount;



	FViewport* InViewport;

	FSocket* Socket;
	//FILE* ImageFile;
	FString myFileName;
	bool Connected;
public:
	UFUNCTION(BlueprintCallable, Category = "NetworkFunction")
		void TakePictureAndSend();
	FRunnableThread* m_RecvThread;

	//收消息(子线程)
	UFUNCTION(BlueprintCallable, Category = "MySocket")
		bool SocketReceive();


	UFUNCTION(BlueprintCallable, Category = "MySocket")
		bool ThreadEnd();

	UFUNCTION(BlueprintCallable, Category = "MySocket")
		void SetIsNewDataComingInReceiveThread();



	UFUNCTION(BlueprintCallable, Category = "MySocket")
		void ConnectServer();

	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float axisPoint1_x;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float axisPoint2_x;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float fingerPoint1_x;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float fingerPoint2_x;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float axisPoint1_y;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float axisPoint2_y;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float fingerPoint1_y;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		float fingerPoint2_y;

	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		int32 FixWidth = 640;

	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		int32 FixHeight = 480;

	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
	    bool IsNewDataComing = false;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		uint8 IP0;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		uint8 IP1;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		uint8 IP2;
	UPROPERTY(BlueprintReadWrite, Category = "MySocket")
		uint8 IP3;
	
};
