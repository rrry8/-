#include<DxLib.h>
#include "PadInput.h"
#include "../../_debug/_DebugConOut.h"

PadInput::PadInput()
{
	KeyConfig_SetDefault();
	KeyConfig_Load();

	inputTbl_ = {
		{InputID::Up,    nowKey_[0]},
		{InputID::Down,  nowKey_[1]},
		{InputID::Left,  nowKey_[2]},
		{InputID::Right, nowKey_[3]},
		{InputID::Menu,  nowKey_[11]},
		{InputID::Decide,nowKey_[4]},
		{InputID::Cancel,nowKey_[5]},
		{InputID::BtnS,  nowKey_[8]},
		{InputID::BtnN,  nowKey_[9]},
		{InputID::Reset, nowKey_[10]},
		{InputID::Change, nowKey_[6]},
		{InputID::Lock, nowKey_[7]},
	};
	for (auto id : InputID())
	{
		cntData_.try_emplace(id, TrgBool{ 0,0 });
	}
}

PadInput::~PadInput()
{
}

void PadInput::Update(void)
{
	//右スティックの入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &rStick_Input);

	inputData_ = GetJoypadInputState(DX_INPUT_PAD1);//できれば可変にする
	KeyConfig_Load();

	for (auto id : InputID())
	{
		cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
		cntData_[id][static_cast<int>(Trg::Now)] = ((inputData_ & inputTbl_[id]) > 0);//＆をとることで数値が残る(ボタンによって異なる)

	}

	for (PInfo = padInfoTable_; PInfo->Name != NULL; PInfo++)
	{
		//何かのキーが押された
		if ((inputData_ & PInfo->PadInput) != 0)
		{
			TRACE("Key %s\n", PInfo->Name);
		}

		//TRACE("右スティック x%d:y%d:z%d\n", rStick_Input.Rx, rStick_Input.Ry, rStick_Input.Rz);
		
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
}

void PadInput::Draw(void)
{
	auto Color = GetColor(255, 255, 255);
	DrawFormatString(0, 0, Color, "X:%d Y:%d Z:%d",
		rStick_Input.X, rStick_Input.Y, rStick_Input.Z);
	DrawFormatString(0, 16, Color, "Rx:%d Ry:%d Rz:%d",
		rStick_Input.Rx, rStick_Input.Ry, rStick_Input.Rz);
	DrawFormatString(0, 32, Color, "Slider 0:%d 1:%d",
		rStick_Input.Slider[0], rStick_Input.Slider[1]);
	DrawFormatString(0, 48, Color, "POV 0:%d 1:%d 2:%d 3:%d",
		rStick_Input.POV[0], rStick_Input.POV[1],
		rStick_Input.POV[2], rStick_Input.POV[3]);
	DrawString(0, 64, "Button", Color);
	for (int i = 0; i < 32; i++)
	{
		DrawFormatString(64 + i % 8 * 64, 64 + i / 8 * 16, Color,
			"%2d:%d", i, rStick_Input.Buttons[i]);
	}
}

int PadInput::KeyChange(int oldKey)
{
	int g_OldKey = oldKey;
	// 一致するキーコード情報をテーブルから探す
	for (PInfo = padInfoTable_; PInfo->Name != NULL; PInfo++)
	{
		//何かのキーが押された
		if ((inputData_ & PInfo->PadInput) != 0 )
		{
			TRACE("Key %s\n", PInfo->Name);
			//押されたキーを戻り値に
			newInputKey_ = PInfo->PadInput;
			return newInputKey_;
			break;
		}
		//if (CheckHitKey(KEY_INPUT_SPACE))//	変更せず、元のキーを入れる
		//{
		//	//変更前のキーを戻り値に
		//	return g_OldKey;
		//	break;
		//}
	}
}

int PadInput::GetNowKey(int num)
{
	return nowKey_[num];
}
void PadInput::SetNewKey(int num, int newKey)
{
	int wk = 0;//作業用変数

	//キーの重複を検知したらキーを入れ替え
	for (int i = 0; i < PAD_INPUTID_MAX; i++)
	{
		//TRACE("nowKey[%d]:%d,,,newkey:%d,,,nowKey[%d]:%d\n", i, nowKey_[i], newKey, num, nowKey_[num]);
		if (newKey == nowKey_[i])
		{
			warningFlg = true;
			wk = nowKey_[num];
			nowKey_[num] = nowKey_[i];
			data[num]->code = nowKey_[i];
			nowKey_[i] = wk;

			break;
		}
	}

	if (!warningFlg)
	{
		// 一致するキーコード情報をテーブルから探す
		for (PInfo = padInfoTable_; PInfo->Name != nullptr; PInfo++)
		{

			if (newKey == PInfo->PadInput)
			{
				data[num]->Name = newKeyName_ = PInfo->Name;
				nowKey_[num] = newKey;
				data[num]->code = newKey;
			}
		}
	}

}

NewKeyData* PadInput::GetNewKeyData(int num)
{
	return  data[num];
}

bool PadInput::GetWarningFlg(void)
{
	return warningFlg;
}

int PadInput::KeyConfig_Save(/*const char* FilePath*/)
{
	FILE* fp;
	int err;
	// 対応情報を保存するファイルを開く
	err = fopen_s(&fp, "./source/Data/keyconfig/keyConfig_pad.txt", "wt");
	if (err != 0)
	{
		// 開けなかったら何もせずに終了
		TRACE("ファイルの書き込みに失敗しました\n");
		return FALSE;
	}

	// ゲームの各入力とキーやパッドなどの入力との対応情報をファイルに書き出す
	for (int i = 0; i < PAD_INPUTID_MAX; i++)
	{
		fprintf(fp, "Key%d = %d \n",
			i,
			nowKey_[i]
		);
	}

	// ファイルを閉じる
	fclose(fp);


	// 成功終了
	return TRUE;
}

int PadInput::KeyConfig_Load(/*const char* FilePath*/)
{
	FILE* fp;
	int err;

	// 対応情報を保存したファイルを開く
	err = fopen_s(&fp, "./source/Data/keyconfig/keyConfig_pad.txt", "rt");
	if (err != 0)
	{
		// 開けなかった場合は何もせずに終了
		TRACE("ファイルの読み込みに失敗しました\n");
		return FALSE;
	}

	// ゲームの各入力とキーやパッドなどの入力との対応情報をファイルから読み込む
	for (int i = 0; i < PAD_INPUTID_MAX; i++)
	{
		fscanf_s(fp, "Key%d = %d\n",
			&i,
			&nowKey_[i]
		);
	}

	// ファイルを閉じる
	fclose(fp);

	inputTbl_ = {
		{InputID::Up,    nowKey_[0]},
		{InputID::Down,  nowKey_[1]},
		{InputID::Left,  nowKey_[2]},
		{InputID::Right, nowKey_[3]},
		{InputID::Menu,  nowKey_[11]},
		{InputID::Decide,nowKey_[4]},
		{InputID::Cancel,nowKey_[5]},
		{InputID::BtnS,  nowKey_[8]},
		{InputID::BtnN,  nowKey_[9]},
		{InputID::Reset, nowKey_[10]},
		{InputID::Change, nowKey_[6]},
		{InputID::Lock, nowKey_[7]},
	};


	for (auto id : InputID())
	{
		cntData_.try_emplace(id, TrgBool{ 0,0 });
	}

	for (int i = 0; i < PAD_INPUTID_MAX; i++)
	{
		data[i] = new NewKeyData();
		data[i]->code = nowKey_[i];
		// 一致するキーコード情報をテーブルから探す
		for (PInfo = padInfoTable_; PInfo->Name != NULL; PInfo++)
		{
			if (nowKey_[i] == PInfo->PadInput)
			{
				data[i]->Name = newKeyName_ = PInfo->Name;
			}
		}
	}

	// 成功終了
	return TRUE;
}

void PadInput::KeyConfig_SetDefault(void)
{
	nowKey_[0] = PAD_INPUT_UP;
	nowKey_[1] = PAD_INPUT_DOWN;
	nowKey_[2] = PAD_INPUT_LEFT;
	nowKey_[3] = PAD_INPUT_RIGHT;
	nowKey_[4] = PAD_INPUT_1;//a
	nowKey_[5] = PAD_INPUT_2;//b
	nowKey_[6] = PAD_INPUT_3;//x
	nowKey_[7] = PAD_INPUT_4;//y
	nowKey_[8] = PAD_INPUT_6;//l1
	nowKey_[9] = PAD_INPUT_5;//r1
	nowKey_[10] = PAD_INPUT_7;//optionL
	nowKey_[11] = PAD_INPUT_8;//optionR
	nowKey_[12] = PAD_INPUT_9;//?
	nowKey_[13] = PAD_INPUT_10;//?
}

bool PadInput::GetKeyChangeFlg(void)
{
	return keyChangeFlg;;
}

void PadInput::SetKeyChangeFlg(bool flg)
{
	keyChangeFlg = flg;
}

DINPUT_JOYSTATE PadInput::GetRstick()
{
	return rStick_Input;
}
