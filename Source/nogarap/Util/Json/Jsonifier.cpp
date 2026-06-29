#include "Jsonifier.h"

TSharedPtr<FJsonObject> UJsonifier::FromString(const FString& JsonString, bool& bOutSuccess)
{
	TSharedPtr<FJsonObject> ReturnJsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), ReturnJsonObject))
	{
		bOutSuccess = false;
		return nullptr;
	}

	bOutSuccess = true;
	return ReturnJsonObject;
}

FString UJsonifier::ToString(const TSharedPtr<FJsonObject>& JsonObject, bool& bOutSuccess)
{
	FString JsonString;

	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		bOutSuccess = false;
		return "";
	}

	bOutSuccess = true;
	//OutInfoMessage = FString::Printf(TEXT("Write Json succeeded - '%s'"), *JsonString);
	return JsonString;
}
