#pragma once
// #include "Player/TGPlayerState.h"

class TGUtils
{
public:
	template <typename T>
	static T* GetTGPlayerComponent(AActor* SomeActor)
	{
		if (!SomeActor) return nullptr;

		const auto Component = SomeActor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}

	//bool static AreEnemies(AController* Controller1, AController* Controller2)
	//{
	//	if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;
//
	//	const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
	//	const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);
//
	//	return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	//}

	static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};
