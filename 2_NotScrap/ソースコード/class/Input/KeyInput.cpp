#include <DxLib.h>
#include"KeyInput.h"
#include "../../_debug/_DebugConOut.h"



KeyInput::KeyInput()
{
	KeyConfig_SetDefault();
	KeyConfig_Load();
	
	inputTbl_ = {
		{InputID::Up,    nowKey_[0]},
		{InputID::Down,  nowKey_[1]},
		{InputID::Left,  nowKey_[2]},
		{InputID::Right, nowKey_[3]},
		{InputID::Menu,  nowKey_[4]},
		{InputID::Decide,MOUSE_INPUT_LEFT},
		{InputID::Cancel,MOUSE_INPUT_RIGHT},
		{InputID::BtnS,  MOUSE_INPUT_RIGHT},
		{InputID::BtnN,  MOUSE_INPUT_LEFT},
		{InputID::Reset, nowKey_[5]},
		{InputID::Change, nowKey_[6]},
		{InputID::Lock, nowKey_[7]},
	};

	
	for (auto id : InputID())
	{
		cntData_.try_emplace(id, TrgBool{ 0,0 });
	}

	//ringBuf_ = ringBuf_->Create(20);
	/*for (auto id : inputTbl_)
	{
		cntData_.try_emplace(id.first, TrgBool{ 0,0 });
	}*/
	warningFlg = false;
	cnt = 0;
}

KeyInput::~KeyInput()
{
	//RingBuf::Destroy(ringBuf_);
}

void KeyInput::Update(void)
{
	KeyConfig_Load();

	GetHitKeyStateAll(KeyData_.data());
	//マウス押下状態を取得
	auto mouseInput = GetMouseInput();

	for (auto id : InputID())
	{
		if (id==InputID::Decide||
			id==InputID::Cancel ||
			id==InputID::BtnN || 
			id==InputID::BtnS)//!mouseInput)
		{
			cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
			cntData_[id][static_cast<int>(Trg::Now)] = ((mouseInput & inputTbl_[id]) > 0);
		}
		else
		{
			cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
			cntData_[id][static_cast<int>(Trg::Now)] = KeyData_[inputTbl_[id]];
		}
	}
	
	if (warningFlg)
	{
		if (cnt > 60)
		{
			warningFlg = false;
			cnt = 0;
		}
		cnt++;
	}
	

	//WriteCmd();
}

void KeyInput::Draw(void)
{
	
}
int KeyInput::KeyChange(int oldKey)
{
	int g_OldKey = oldKey;
	// 一致するキーコード情報をテーブルから探す
	for (KInfo = keyInfoTable_; KInfo->Name != NULL; KInfo++)
	{
		//何かのキーが押された　かつ　それがマウスのクリックではない時中に入る
		if (CheckHitKey(KInfo->KeyInput)&& !cntData_[InputID::Decide][static_cast<int>(Trg::Now)] && !cntData_[InputID::Decide][static_cast<int>(Trg::Old)]//決定ボタンを押した時
			&& !cntData_[InputID::Cancel][static_cast<int>(Trg::Now)] && !cntData_[InputID::Cancel][static_cast<int>(Trg::Old)])
		{
			//TRACE("Key %s\n", KInfo->Name);
			//押されたキーを戻り値に
			newInputKey_ = KInfo->KeyInput;
			keyChangeFlg = true;
			return newInputKey_;
			break;
		}
		
	}
	
}
int KeyInput::GetNowKey(int num)
{
	return nowKey_[num];
}
void KeyInput::SetNewKey(int num,int newKey)
{
	int wk = 0;//作業用変数
	
	//キーの重複を検知したらキーを入れ替え
	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		//TRACE("nowKey[%d]:%d,,,newkey:%d,,,nowKey[%d]:%d\n", i, nowKey_[i], newKey, num, nowKey_[num]);
		if (newKey == nowKey_[i])
		{
			TRACE("重複！\n");
			TRACE("nowKey[%d]:%d<-->nowKey[%d]:%d\n", i, nowKey_[i], num, nowKey_[num]);
			warningFlg = true;
			wk = nowKey_[num];
			nowKey_[num] = nowKey_[i];
			data[num]->code = nowKey_[i];
			nowKey_[i] = wk;
			
			TRACE("キーの入れ替え完了\n");
			TRACE("nowKey[%d]:%d<-->nowKey[%d]:%d\n", i, nowKey_[i], num, nowKey_[num]);
			break;
		}
		else
		{
			//TRACE("重複なし。\n");
		}
	}
	
	if (!warningFlg)
	{
		// 一致するキーコード情報をテーブルから探す
		for (KInfo = keyInfoTable_; KInfo->Name != nullptr; KInfo++)
		{

			if (newKey == KInfo->KeyInput)
			{
				data[num]->Name = newKeyName_ = KInfo->Name;
				nowKey_[num] = newKey;
				data[num]->code = newKey;
			}
		}
	}
	
	
	
}
NewKeyData* KeyInput::GetNewKeyData(int num)
{
	return data[num];
}
bool KeyInput::GetWarningFlg()
{
	return warningFlg;
}
int KeyInput::KeyConfig_Save(/*const char* FilePath*/)
{
	FILE* fp;
	int err;
	// 対応情報を保存するファイルを開く
	err = fopen_s(&fp, "./source/Data/keyconfig/keyConfig.txt", "wt");
	if (err != 0)
	{
		// 開けなかったら何もせずに終了
		TRACE("ファイルの書き込みに失敗しました\n");
		return FALSE;
	}

	// ゲームの各入力とキーやパッドなどの入力との対応情報をファイルに書き出す
	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		fprintf(fp, "Key%d = %d \n",
			i,
			nowKey_[i]
		);
	}

	// ファイルを閉じる
	fclose(fp);


	// 成功終了
	//TRACE("ファイルの書き込み完了\n");
	return TRUE;
}
int KeyInput::KeyConfig_Load(/*const char* FilePath*/)
{
	FILE* fp;
	int err;

	// 対応情報を保存したファイルを開く
	err = fopen_s(&fp, "./source/Data/keyconfig/keyConfig.txt", "rt");
	if (err != 0)
	{
		// 開けなかった場合は何もせずに終了
		TRACE("ファイルの読み込みに失敗しました\n");
		return FALSE;
	}

	// ゲームの各入力とキーやパッドなどの入力との対応情報をファイルから読み込む
	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		fscanf_s(fp, "Key%d = %d\n",
			&i,
			&nowKey_[i]
		);
	}

	// ファイルを閉じる
	//TRACE("ファイルの読み込み完了\n");
	fclose(fp);

	inputTbl_ = {
		{InputID::Up,    nowKey_[0]},
		{InputID::Down,  nowKey_[1]},
		{InputID::Left,  nowKey_[2]},
		{InputID::Right, nowKey_[3]},
		{InputID::Menu,  nowKey_[4]},
		{InputID::Decide,MOUSE_INPUT_LEFT},
		{InputID::Cancel,MOUSE_INPUT_RIGHT},
		{InputID::BtnS,  MOUSE_INPUT_RIGHT},
		{InputID::BtnN,  MOUSE_INPUT_LEFT},
		{InputID::Reset, nowKey_[5]},
		{InputID::Change, nowKey_[6]},
		{InputID::Lock, nowKey_[7]},
	};


	for (auto id : InputID())
	{
		cntData_.try_emplace(id, TrgBool{ 0,0 });
	}

	for (int i = 0; i < INPUT_ID_MAX; i++)
	{
		data[i] = new NewKeyData();
		data[i]->code = nowKey_[i];
		// 一致するキーコード情報をテーブルから探す
		for (KInfo = keyInfoTable_; KInfo->Name != NULL; KInfo++)
		{
			if (nowKey_[i] == KInfo->KeyInput)
			{
				data[i]->Name = newKeyName_ = KInfo->Name;
			}
		}
	}

	// 成功終了
	return TRUE;
}
void KeyInput::KeyConfig_SetDefault(void)
{
	nowKey_[0] = KEY_INPUT_W;
	nowKey_[1] = KEY_INPUT_S;
	nowKey_[2] = KEY_INPUT_A;
	nowKey_[3] = KEY_INPUT_D;
	nowKey_[4] = KEY_INPUT_ESCAPE;
	nowKey_[5] = KEY_INPUT_R;
	nowKey_[6] = KEY_INPUT_SPACE;
	nowKey_[7] = KEY_INPUT_LSHIFT;
}
bool KeyInput::GetKeyChangeFlg()
{
	return keyChangeFlg;
}

void KeyInput::SetKeyChangeFlg(bool flg)
{
	keyChangeFlg = flg;
}
