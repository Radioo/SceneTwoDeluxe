#include "pch.h"

const enum SCENE : unsigned short
{
	STAGE_SCENE = 0x1A,
	RESULT_SCENE = 0xB,
	MUSIC_SELECT_SCENE = 0x11,
	CARD_OUT_SCENE = 0x13,
	MODE_SELECT_SCENE = 0x8,
	DAN_SELECT_SCENE = 0x9,  // called with 0x3 before 0x9
	DAN_STAGE_SCENE = 0x18,
	DAN_RESULT_SCENE = 0xA,
	TITLE_SCENE = 0x16, // called with 0x1 before 0x16
	CARD_IN_SCENE = 0x14,
	ATTRACT_SCREEN = 0x17
};