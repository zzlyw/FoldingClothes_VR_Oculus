// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureScreen.h"
#include "ImageUtils.h"  
#include "IImageWrapper.h"  
#include "IImageWrapperModule.h"  

EImageFormatType format;
int32 quality;
FString fileName;
FDelegateHandle delegateHandle;

void ACaptureScreen::OnScreenshotCapturedInternal(int32 Width, int32 Height, const TArray<FColor>& Bitmap)
{

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper;
	if (format == EImageFormatType::PNG)
	{
		TArray<uint8> PNGData;
		TArray<FColor> BitmapCopy(Bitmap);
		FImageUtils::CompressImageArray(Width, Height, BitmapCopy, PNGData);

		fileName += ".png";
		FFileHelper::SaveArrayToFile(PNGData, *fileName);
	}
	else if (format == EImageFormatType::JPEG)
	{
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
		const TArray<uint8>& JPEGData = ImageWrapper->GetCompressed(quality);
		fileName += ".jpg";
		FFileHelper::SaveArrayToFile(JPEGData, *fileName);
	}
	else if (format == EImageFormatType::EXR)
	{
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::EXR);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
		const TArray<uint8>& Data = ImageWrapper->GetCompressed(quality);
		fileName += ".exr";
		FFileHelper::SaveArrayToFile(Data, *fileName);
	}
	else
	{
		//默认使用Jpg格式  
		ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);
		const TArray<uint8>& JPEGData = ImageWrapper->GetCompressed(quality);
		fileName += ".jpg";
		FFileHelper::SaveArrayToFile(JPEGData, *fileName);
	}
	UGameViewportClient::OnScreenshotCaptured().Remove(delegateHandle);
	ImageWrapper.Reset();

}

void ACaptureScreen::TakeScreenShot(const FString fineName /*= FString(TEXT("ScreenShot"))*/, EImageFormatType _format /*= EImageFormatType::PNG*/,
	int32 _quality /*= 100*/, bool bInsertTimeStampToFileName /*= true*/, bool bInShowUI /*= true*/, bool bAddFilenameSuffix /*= true*/)
{



	FString filename = fineName;

	format = _format;
	quality = _quality;

	//名字后面加上现在的时间  
	if (bInsertTimeStampToFileName)
	{
		filename += "_" + FDateTime::Now().ToString();
	}
	//名字后面加上下划线  
	if (bAddFilenameSuffix)
	{
		filename += "_";
	}

	fileName = FPaths::ProjectSavedDir() + "ScreenShots/Windows/" + filename;

	if (!UGameViewportClient::OnScreenshotCaptured().IsBound())
	{
		delegateHandle = UGameViewportClient::OnScreenshotCaptured().AddStatic(&OnScreenshotCapturedInternal);

		FScreenshotRequest::RequestScreenshot(filename, bInShowUI, bAddFilenameSuffix);
		//加这个是高分辨率截图但是会有锯齿  
		//GEngine->GameViewport->Exec(nullptr, TEXT("HighResShot 1080x720"), *GLog);  
	}
}

// Sets default values
ACaptureScreen::ACaptureScreen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACaptureScreen::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACaptureScreen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

