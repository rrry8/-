#pragma once
#include<type_traits>

//ƒL[‚Ìí—Ş
enum class KeyID
{
	Up,
	Down,
	Left,
	Right,
	Decide,
	Cancel,
	Pause,
	Button,
	Max
};

static KeyID begin(KeyID) { return KeyID::Up; };
static KeyID end(KeyID) { return KeyID::Max; };
static KeyID operator++(KeyID& state) { return state = static_cast<KeyID>(std::underlying_type<KeyID>::type(state) + 1); };
static KeyID operator*(const KeyID& state) { return state; };
