#pragma once
#include "NativeGameplayTags.h"

/*
* Gameplay Tag를 관리하는 파일입니다.
* CPP에서 태그를 이용하려면 에디터에서 설정하는 것이 아니라 CPP에서 설정해야 합니다.
* 작성자 : 임희섭
* 작성일 : 25/11/05
*/
namespace AruaGamePlayTags
{
	/*Player State Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_LockOn)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Roll)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Skill_Whirlwind)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_Skill_ChargeAttack)

	/*Ability Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LockOn)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Roll)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LockOnDodge)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Whirlwind)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ChargeAttack)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_AddChargeCount)

	/*Condition State Tags*/
	// 상태 이상 관련 태그
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Condition_Immunity) // 면역 상태, 대미지도 입지 않음 25/11/06

	/*Damage Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage_Base)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage_Multiplier)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Heal_Consumable)

	/*Event Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Dead)
	//ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Quest_Start)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Quest_Update)
	//ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Quest_Complete)
		ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Event_Interaction_Start)

	/*WidgetStack Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_Modal)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameMenu)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameHUD)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WidgetStack_GameFrontend)

	/*Widget Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Widget_Title)
	
	/*Player Hit Tag*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Tag)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Character_AttackHit)

	/* Quest Tags */
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(QuestItem);

	/*Weapon Tag*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Weapon_None)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_Weapon_Sword)

	/*Gameplay Cue Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Monster_Hit)
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Player_Attack)
}
