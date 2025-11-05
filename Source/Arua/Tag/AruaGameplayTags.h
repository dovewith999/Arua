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

	/*Ability Tags*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_LockOn)

	/*Input Tags for Abilities*/
	ARUA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LockOn)
}
