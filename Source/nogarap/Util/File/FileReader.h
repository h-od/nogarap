#pragma once

#include "UObject/Object.h"

#include "FileReader.generated.h"

UCLASS()
class NOGARAP_API UFileReader : public UObject
{
	GENERATED_BODY()

public:
	static FString Read(const FString& FilePath, bool& bOutSuccess);
};
