// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureScreen.generated.h"

UENUM(BlueprintType)
enum class EImageFormatType : uint8
{
	/** Portable Network Graphics. */
	PNG,

	/** Joint Photographic Experts Group. */
	JPEG,

	/** OpenEXR (HDR) image file format. */
	EXR,
};

UCLASS()
class CLOTHBALL_API ACaptureScreen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACaptureScreen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static void OnScreenshotCapturedInternal(int32 Width, int32 Height, const TArray<FColor>& Bitmap);
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"), Category = "TalkCameraDebug")
	void TakeScreenShot(const FString fineName = FString(TEXT("ScreenShot")), EImageFormatType _format = EImageFormatType::PNG,
			int32 _quality = 100, bool bInsertTimeStampToFileName = false, bool bInShowUI = false, bool bAddFilenameSuffix = false);
	
	
};
