#include "Tag/AruaGameplayTags.h"

namespace AruaGamePlayTags
{
	/*Player Ability Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dead, "Player.Ability.Dead")

	/*Player State Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Player_State_LockOn, "Player.State.LockOn")
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Roll, "Player.State.Roll")
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Skill_Whirlwind, "Player.State.Skill.Whirlwind")
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Skill_ChargeAttack, "Player.State.Skill.ChargeAttack")
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Dead, "Player.State.Dead")

	/*Ability Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Ability_LockOn, "Ability.LockOn")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Roll, "Ability.Roll")
	UE_DEFINE_GAMEPLAY_TAG(Ability_LockOnDodge, "Ability.LockOnDodge")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Whirlwind, "Ability.Whirlwind")
	UE_DEFINE_GAMEPLAY_TAG(Ability_ChargeAttack, "Ability.ChargeAttack")
	UE_DEFINE_GAMEPLAY_TAG(Ability_AddChargeCount, "Ability.AddChargeCount")
	UE_DEFINE_GAMEPLAY_TAG(Ability_PickUp, "Ability.PickUp")

	/*Condition State Tags*/
	// 상태 이상 관련 태그
	UE_DEFINE_GAMEPLAY_TAG(Condition_Immunity, "Condition.Immunity")

	/*Damage Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage_Base, "Data.Damage.Base")
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage_Multiplier, "Data.Damage.Multiplier")
	UE_DEFINE_GAMEPLAY_TAG(Data_Heal_Consumable, "Data.Heal.Consumable")

	/*Event Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Event_Dead, "Event.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Dead, "Player.Event.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Event_Quest_Update, "Event.Quest.Update")
	UE_DEFINE_GAMEPLAY_TAG(Event_Interaction_Start, "Event.Interaction.Start")
	
	/*WidgetStack Tags*/
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_Modal, "WidgetStack.Modal")
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameMenu, "WidgetStack.GameMenu")
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameHUD, "WidgetStack.GameHUD")
	UE_DEFINE_GAMEPLAY_TAG(WidgetStack_GameFrontend, "WidgetStack.GameFrontend")

	/*Widget Tags*/
	UE_DEFINE_GAMEPLAY_TAG(Widget_Title, "Widget.Title")

	/*Player Hit Tag*/
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Tag,"Character.State.Tag")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Character_AttackHit,"GameplayCue.Character.AttackHit")

	/* Quest Tags */
	UE_DEFINE_GAMEPLAY_TAG(QuestItem, "QuestItem")

	/*Weapon Tag*/
	UE_DEFINE_GAMEPLAY_TAG(Character_Weapon_None,"Character.Weapon.None")
	UE_DEFINE_GAMEPLAY_TAG(Character_Weapon_Sword,"Character.Weapon.Sword")

	/*Gameplay Cue Tags*/
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Monster_Hit, "GameplayCue.Monster.Hit")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Attack, "GameplayCue.Player.Attack")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Attack_Special, "GameplayCue.Player.Attack.Special")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Player_Attack_Impact, "GameplayCue.Player.Attack.Impact")
}