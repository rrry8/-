#pragma once
#include "../Common/Geometry.h"
#include "Controller.h"
// キーボードの１キー辺りの情報
struct PADINFO
{
    int         PadInput;           // 入力コード( KEY_INPUT_LEFT など )
    const char* Name;               // キーの名前
};
// キーコードの情報
static PADINFO padInfoTable_[] =
{
    PAD_INPUT_UP,           "UP",        //UP
    PAD_INPUT_DOWN,         "DOWN",     //DOWN
    PAD_INPUT_LEFT,         "LEFT",     //LEFT
    PAD_INPUT_RIGHT,        "RIGHT",    //RIGHT
    PAD_INPUT_1,            "A",        // A
    PAD_INPUT_2,            "B",        // B
    PAD_INPUT_3,            "X",        // X 
    PAD_INPUT_4,            "Y",        // Y
    PAD_INPUT_5,            "L1",       // L1
    PAD_INPUT_6,            "R1",       // R1
    PAD_INPUT_7,            "OptionL",  // 真ん中の左のボタン
    PAD_INPUT_8,            "OptionR",  // 真ん中の右のボタン
    PAD_INPUT_9,            "9",        // 
    PAD_INPUT_10,           "10",       // 
    PAD_INPUT_11,           "11",       // 
    PAD_INPUT_12,           "12",       // 
    PAD_INPUT_13,           "13",       //
    PAD_INPUT_14,           "14",       // 
    PAD_INPUT_15,           "15",       // 
    PAD_INPUT_16,           "16",       //
    PAD_INPUT_17,           "17",       // 
    PAD_INPUT_18,           "18",       // 
    PAD_INPUT_19,           "19",       // 
    PAD_INPUT_20,           "20",       // 
    PAD_INPUT_21,           "21",       // 
    PAD_INPUT_22,           "22",       // 
    PAD_INPUT_23,           "23",       // 
    PAD_INPUT_24,           "24",       // 
    PAD_INPUT_25,           "25",       // 
    PAD_INPUT_26,           "26",       // 
    PAD_INPUT_27,           "27",       // 
    PAD_INPUT_28,           "28",       // 

    -1,                     NULL,
};
class PadInput :
    public Controller
{
public:
    PadInput();
    virtual ~PadInput();

   void Update(void)override;
   void Draw(void)override;
   int KeyChange(int oldKey)override;
   int GetNowKey(int num)override;
   void SetNewKey(int num, int newKey)override;
   NewKeyData* GetNewKeyData(int num)override;
   bool GetWarningFlg(void)override;
   CntType GetCntType(void)override {
       return CntType::Pad;
   };
   int KeyConfig_Save(/*const char* FilePath*/) override;   // キーコンフィグ設定をファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
   int KeyConfig_Load(/*const char* FilePath*/) override;   // キーコンフィグ設定をファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
   void KeyConfig_SetDefault(void) override;             // キーコンフィグ設定をデフォルトにする
   bool GetKeyChangeFlg(void);//キー変更フラグを返す
   void SetKeyChangeFlg(bool flg);
   DINPUT_JOYSTATE GetRstick();
private:
    std::array<char, 256U>KeyData_;
    std::map<InputID, unsigned int> inputTbl_;
    DINPUT_JOYSTATE rStick_Input;//右スティックの入力情報
    int nowKey_[PAD_INPUTID_MAX];//現在のキー情報
    int newInputKey_;//新しく入力されたキー
    int inputData_;//PADの入力情報
    const char* newKeyName_;//新しく入力されたキー
    NewKeyData* data[PAD_INPUTID_MAX];//キーの情報(code,name)
    PADINFO* PInfo;//全キーの情報(code,name)
    int cnt;//warningの表示の経過時間
protected:
    Vector2 pos_;
    signed int speed_;
};

