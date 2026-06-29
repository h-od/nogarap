#include "FileWrite.h"

void UFileWrite::Write(const FString& FilePath, const FString& String, bool& bOutSuccess)
{
	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		bOutSuccess = false;
		return;
	}

	bOutSuccess = true;
}
