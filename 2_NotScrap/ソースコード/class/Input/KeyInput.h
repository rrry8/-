#pragma once
#include "../Common/Geometry.h"
#include "Controller.h"

// キーボードの１キー辺りの情報
struct KEYINFO
{
    int         KeyInput;           // 入力コード( KEY_INPUT_LEFT など )
    const char* Name;               // キーの名前
};
// キーコードの情報
static KEYINFO keyInfoTable_[] =
{
    KEY_INPUT_BACK,         "BACK",         // バックスペースキー
    KEY_INPUT_TAB,          "TAB",          // タブキー
    KEY_INPUT_RETURN,       "ENTER",        // エンターキー

    KEY_INPUT_LSHIFT,       "L SHIFT",      // 左シフトキー
    KEY_INPUT_RSHIFT,       "R SHIFT",      // 右シフトキー
    KEY_INPUT_LCONTROL,     "L CONTROL",    // 左コントロールキー
    KEY_INPUT_RCONTROL,     "R CONTROL",    // 右コントロールキー
    KEY_INPUT_ESCAPE,       "ESCAPE",       // エスケープキー
    KEY_INPUT_SPACE,        "SPACE",        // スペースキー
    KEY_INPUT_PGUP,         "PAGE UP",      // ＰａｇｅＵＰキー
    KEY_INPUT_PGDN,         "PAGE DOWN",    // ＰａｇｅＤｏｗｎキー
    KEY_INPUT_END,          "END",          // エンドキー
    KEY_INPUT_HOME,         "HOME",         // ホームキー
    KEY_INPUT_LEFT,         "←",         // 左キー
    KEY_INPUT_UP,           "↑",           // 上キー
    KEY_INPUT_RIGHT,        "→",        // 右キー
    KEY_INPUT_DOWN,         "↓",         // 下キー
    KEY_INPUT_INSERT,       "INSERT",       // インサートキー
    KEY_INPUT_DELETE,       "DELETE",       // デリートキー

    KEY_INPUT_MINUS,        "-",            // －キー
    KEY_INPUT_YEN,          "\\",           // ￥キー
    KEY_INPUT_PREVTRACK,    "^",            // ＾キー
    KEY_INPUT_PERIOD,       ".",            // ．キー
    KEY_INPUT_SLASH,        "/",            // ／キー
    KEY_INPUT_LALT,         "L ALT",        // 左ＡＬＴキー
    KEY_INPUT_RALT,         "R ALT",        // 右ＡＬＴキー
    KEY_INPUT_SCROLL,       "SCROLL LOCK",  // ScrollLockキー
    KEY_INPUT_SEMICOLON,    ";",            // ；キー
    KEY_INPUT_COLON,        ":",            // ：キー
    KEY_INPUT_LBRACKET,     "[",            // ［キー
    KEY_INPUT_RBRACKET,     "]",            // ］キー
    KEY_INPUT_AT,           "@",            // ＠キー
    KEY_INPUT_BACKSLASH,    "BACK SLASH",   // ＼キー
    KEY_INPUT_COMMA,        ",",            // ，キー
    KEY_INPUT_KANJI,        "KANJI",        // 漢字キー
    KEY_INPUT_CONVERT,      "CONVERT",      // 変換キー
    KEY_INPUT_NOCONVERT,    "NO CONVERT",   // 無変換キー
    KEY_INPUT_KANA,         "KANA",         // カナキー
    KEY_INPUT_APPS,         "APPS",         // アプリケーションメニューキー
    KEY_INPUT_CAPSLOCK,     "CAPS LOCK",    // CaspLockキー
    KEY_INPUT_SYSRQ,        "PRINT SCREEN", // PrintScreenキー
    KEY_INPUT_PAUSE,        "PAUSE BREAK",  // PauseBreakキー
    KEY_INPUT_LWIN,         "L WIN",        // 左Ｗｉｎキー
    KEY_INPUT_RWIN,         "R WIN",        // 右Ｗｉｎキー

    KEY_INPUT_NUMLOCK,      "NUM LOCK",     // テンキー０
    KEY_INPUT_NUMPAD0,      "NUMPAD 0",     // テンキー０
    KEY_INPUT_NUMPAD1,      "NUMPAD 1",     // テンキー１
    KEY_INPUT_NUMPAD2,      "NUMPAD 2",     // テンキー２
    KEY_INPUT_NUMPAD3,      "NUMPAD 3",     // テンキー３
    KEY_INPUT_NUMPAD4,      "NUMPAD 4",     // テンキー４
    KEY_INPUT_NUMPAD5,      "NUMPAD 5",     // テンキー５
    KEY_INPUT_NUMPAD6,      "NUMPAD 6",     // テンキー６
    KEY_INPUT_NUMPAD7,      "NUMPAD 7",     // テンキー７
    KEY_INPUT_NUMPAD8,      "NUMPAD 8",     // テンキー８
    KEY_INPUT_NUMPAD9,      "NUMPAD 9",     // テンキー９
    KEY_INPUT_MULTIPLY,     "NUMPAD *",     // テンキー＊キー
    KEY_INPUT_ADD,          "NUMPAD +",     // テンキー＋キー
    KEY_INPUT_SUBTRACT,     "NUMPAD -",     // テンキー－キー
    KEY_INPUT_DECIMAL,      "NUMPAD .",     // テンキー．キー
    KEY_INPUT_DIVIDE,       "NUMPAD /",     // テンキー／キー
    KEY_INPUT_NUMPADENTER,  "NUMPAD ENTER", // テンキーのエンターキー

    KEY_INPUT_F1,           "F1",           // Ｆ１キー
    KEY_INPUT_F2,           "F2",           // Ｆ２キー
    KEY_INPUT_F3,           "F3",           // Ｆ３キー
    KEY_INPUT_F4,           "F4",           // Ｆ４キー
    KEY_INPUT_F5,           "F5",           // Ｆ５キー
    KEY_INPUT_F6,           "F6",           // Ｆ６キー
    KEY_INPUT_F7,           "F7",           // Ｆ７キー
    KEY_INPUT_F8,           "F8",           // Ｆ８キー
    KEY_INPUT_F9,           "F9",           // Ｆ９キー
    KEY_INPUT_F10,          "F10",          // Ｆ１０キー
    KEY_INPUT_F11,          "F11",          // Ｆ１１キー
    KEY_INPUT_F12,          "F12",          // Ｆ１２キー

    KEY_INPUT_A,            "A",            // Ａキー
    KEY_INPUT_B,            "B",            // Ｂキー
    KEY_INPUT_C,            "C",            // Ｃキー
    KEY_INPUT_D,            "D",            // Ｄキー
    KEY_INPUT_E,            "E",            // Ｅキー
    KEY_INPUT_F,            "F",            // Ｆキー
    KEY_INPUT_G,            "G",            // Ｇキー
    KEY_INPUT_H,            "H",            // Ｈキー
    KEY_INPUT_I,            "I",            // Ｉキー
    KEY_INPUT_J,            "J",            // Ｊキー
    KEY_INPUT_K,            "K",            // Ｋキー
    KEY_INPUT_L,            "L",            // Ｌキー
    KEY_INPUT_M,            "M",            // Ｍキー
    KEY_INPUT_N,            "N",            // Ｎキー
    KEY_INPUT_O,            "O",            // Ｏキー
    KEY_INPUT_P,            "P",            // Ｐキー
    KEY_INPUT_Q,            "Q",            // Ｑキー
    KEY_INPUT_R,            "R",            // Ｒキー
    KEY_INPUT_S,            "S",            // Ｓキー
    KEY_INPUT_T,            "T",            // Ｔキー
    KEY_INPUT_U,            "U",            // Ｕキー
    KEY_INPUT_V,            "V",            // Ｖキー
    KEY_INPUT_W,            "W",            // Ｗキー
    KEY_INPUT_X,            "X",            // Ｘキー
    KEY_INPUT_Y,            "Y",            // Ｙキー
    KEY_INPUT_Z,            "Z",            // Ｚキー

    KEY_INPUT_0,            "0",            // ０キー
    KEY_INPUT_1,            "1",            // １キー
    KEY_INPUT_2,            "2",            // ２キー
    KEY_INPUT_3,            "3",            // ３キー
    KEY_INPUT_4,            "4",            // ４キー
    KEY_INPUT_5,            "5",            // ５キー
    KEY_INPUT_6,            "6",            // ６キー
    KEY_INPUT_7,            "7",            // ７キー
    KEY_INPUT_8,            "8",            // ８キー
    KEY_INPUT_9,            "9",            // ９キー

    -1,                     NULL,
};

class KeyInput :
    public Controller
{
public:
    KeyInput();
    virtual ~KeyInput();
    void Update(void)override;
    void Draw(void)override;
    int KeyChange(int oldKey)override;//キーを押されたらそのキーをnewKeyとして返す
    int GetNowKey(int num)override;//選択した個所のキー情報を取得する
    void SetNewKey(int num, int newKey)override;//newKeyを選択した個所にセット
    NewKeyData * GetNewKeyData(int num)override;//新しく入力されたキーの名前をゲット
    bool GetWarningFlg(void)override;//重複などの警告文が必要な時にフラグがtrueになる
    int KeyConfig_Save(/*const char* FilePath*/); // キーコンフィグ設定をファイルに保存する( 戻り値 : 保存成功：TRUE   保存失敗：FALSE )
    int KeyConfig_Load(/*const char* FilePath*/); // キーコンフィグ設定をファイルから読み込む( 戻り値 : 読み込み成功：TRUE   読み込み失敗：FALSE )
    void KeyConfig_SetDefault(void);       // キーコンフィグ設定をデフォルトにする
    bool GetKeyChangeFlg(void);//キー変更完了フラグを取得
    void SetKeyChangeFlg(bool flg);
    CntType GetCntType(void)override {
        return CntType::Key;
    };
private:
    std::array<char, 256U>KeyData_;
    int MouseX;//マウスの座標
    int MouseY;
    std::map<InputID, unsigned int> inputTbl_;//使用するキーの情報
    int nowKey_[INPUT_ID_MAX];//現在のキー情報
    int newInputKey_;//新しく入力されたキー
    const char* newKeyName_;//新しく入力されたキー
    NewKeyData* data[INPUT_ID_MAX];//キーの情報(code,name)
    KEYINFO* KInfo;//全キーの情報(code,name)
    int cnt;//warningの表示の経過時間
    
};
