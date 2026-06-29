#include "FileReader.h"

#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

FString UFileReader::Read(const FString& FilePath, bool& bOutSuccess)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		bOutSuccess = false;
		return "";
	}

	FString RetString = "";

	if (!FFileHelper::LoadFileToString(RetString, *FilePath))
	{
		bOutSuccess = false;
		return "";
	}

	bOutSuccess = true;
	return RetString;
}
