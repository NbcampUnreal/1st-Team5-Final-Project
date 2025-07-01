// PayRockGames


#include "VOIPLIB.h"

void UVOIPLIB::ClearVoicePackets(UObject* WorldContextObject)
{
	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World))
 	{
		return;
	}
	IOnlineVoicePtr VoiceInterface = Online::GetVoiceInterface(World);
	if(!VoiceInterface.IsValid())
	{
	}

	VoiceInterface->ClearVoicePackets();
}
