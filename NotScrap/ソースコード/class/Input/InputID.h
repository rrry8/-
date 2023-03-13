#pragma once


enum class InputID
{
	Up,		//上移動
	Down,	//下移動
	Left,	//左移動
	Right,	//右移動
	Menu,	//メニューを開く
	Decide,//決定
	Cancel,//キャンセル
	BtnS,//S
	BtnN,//N
	Reset,//リセットボタン	*キーコンフィグで使ってる
	Change,	//ボタン１
	Lock,	//ボタン２
	Max
};




static InputID begin(InputID) { return InputID::Up; };
static InputID end(InputID) { return InputID::Max; };
static InputID operator++(InputID& state) {
	return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); 
};
static InputID operator*(const InputID& state) { return state;};
