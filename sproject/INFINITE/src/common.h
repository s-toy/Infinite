#pragma once

#define SAFE_DELETE(ptr) { if (ptr) { delete ptr; ptr = nullptr; } }
#define SAFE_DELETE_ARRAY(ptr) { if (ptr) { delete[] ptr; ptr = nullptr; } }

enum class EChannelType
{
	BUFFER = 0,
	TEXTURE,
	CUBE_MAP,
	KEYBOARD,
	SOUND,
	SOUND_CLOUD,
	VIDEO
};

enum class EPassType
{
	BUFFER = 0,
	IMAGE,
	SOUND,
	CUBE_MAP
};