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
	UE_DEFINE_GAMEPLAY_TAG(Event_Dead, "Event.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Event_Quest_Update, "Event.Quest.Update")
	
	/*WidgetStack Tags*/
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Modal, "WidgetStack.Modal")
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu, "WidgetStack.GameMenu")
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHUD, "WidgetStack.GameHUD")
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameFrontend, "WidgetStack.GameFrontend")

	/*Widget Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Widget_Title, "Widget.Title")

	/*Player Hit Tag*/
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Tag,"Character.State.Tag")
}