#pragma once

#include "UObject/Object.h"

#include "FileWrite.generated.h"

UCLASS()
class NOGARAP_API UFileWrite : public UObject
{
	GENERATED_BODY()
public:

	static void Write(const FString& FilePath, const FString& String, bool& bOutSuccess);
};
