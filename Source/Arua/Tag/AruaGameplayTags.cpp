#include "Tag/AruaGameplayTags.h"

namespace AruaGamePlayTags
{
	/*Player State Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Player_State_LockOn, "Player.State.LockOn")
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Roll, "Player.State.Roll")
		
	/*Ability Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Ability_LockOn, "Ability.LockOn")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Roll, "Ability.Roll")

	/*Condition State Tags*/
	// 상태 이상 관련 태그
	UE_DEFINE_GAMEPLAY_TAG(Condition_Immunity, "Condition.Immunity")

	/*Damage Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage_Base, "Data.Damage.Base")
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage_Multiplier, "Data.Damage.Multiplier")

	/*Event Tags*/
	 UE_DEFINE_GAMEPLAY_TAG(Event_Death, "Event.Death")
}