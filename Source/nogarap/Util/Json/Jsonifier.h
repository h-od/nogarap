#pragma once

#include "CoreMinimal.h"
#include "Jsonifier.generated.h"

class FJsonObject;

UCLASS()
class NOGARAP_API UJsonifier : public UObject
{
	GENERATED_BODY()
public:
	static TSharedPtr<FJsonObject> FromString(const FString& JsonString, bool& bOutSuccess);

	static FString ToString(const TSharedPtr<FJsonObject>& JsonObject, bool& bOutSuccess);
};
