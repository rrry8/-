#include<DxLib.h>
#include"Geometry.h"
#include <array>

//スクリーンサイズ
constexpr float SCREEN_SIZE_X = 640.0f; //x
constexpr float SCREEN_SIZE_Y = 480.0f; //y

//プリミティブサイズ
constexpr float PRIMITIVE_SIZE_X = 640.0f;  //x
constexpr float PRIMITIVE_SIZE_Y = 480.0f;  //y
constexpr float PRIMITIVE_SIZE_Z = 400;  //z

//カメラ位置
constexpr VECTOR CAMERA_POS = {0.0f,0.0f, -10.0f };

//球情報
constexpr float SPHERE_SIZE = 0.5f;

//ループ回数
constexpr float LOOP_NUM = 64.0f;

void DrawRayMarching(int ps)
{
    std::array<VERTEX2DSHADER, 4> verts{};
    static float angle = 0.0f;
    for (auto& v : verts)
    {
        v.dif = GetColorU8(0, 156, 255, 255);
        v.spc = GetColorU8(0, 0, 0, 0);
        v.pos.z = 0.0f;
        v.rhw = 1.0f;
        v.su = 0.0f;
        v.sv = 0.0f;
    }

    int centerX = static_cast<int>(SCREEN_SIZE_X - PRIMITIVE_SIZE_X) / 2;
    int centerY = static_cast<int>(SCREEN_SIZE_Y - PRIMITIVE_SIZE_Y) / 2;

    float rotX = cosf(angle)*0;
    float rotY = sinf(angle)*0;

    verts[0].pos.x = centerX + rotX;
    verts[0].pos.y = centerY + rotY;

    verts[1].pos.x = centerX + PRIMITIVE_SIZE_X + rotX;
    verts[1].pos.y = centerY + rotY;

    verts[2].pos.x = centerX + rotX;
    verts[2].pos.y = centerY + PRIMITIVE_SIZE_Y + rotY;

    verts[3].pos.x = centerX + PRIMITIVE_SIZE_X + rotX;
    verts[3].pos.y = centerY + PRIMITIVE_SIZE_Y + rotY;

    //UV座標
    verts[0].u = 0.0f;
    verts[0].v = 0.0f;

    verts[1].u = 1.0f;
    verts[1].v = 0.0f;

    verts[2].u = 0.0f;
    verts[2].v = 1.0f;

    verts[3].u = 1.0f;
    verts[3].v = 1.0f;

    SetUsePixelShader(ps);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
    angle +=0.01f;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    SetWindowText("雲");
    ChangeWindowMode(true);
    SetBackgroundColor(0, 156, 255);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    int psCloud = LoadPixelShader("Cloud_ps.pso");          //雲用
    int psSphere = LoadPixelShader("Sphere_ps.pso");   //球用

    //定数バッファ
    int cBuffFloat4 = CreateShaderConstantBuffer(sizeof(FLOAT4) * 4);
    FLOAT4* writerFloat4 = static_cast<FLOAT4*>(GetBufferShaderConstantBuffer(cBuffFloat4));

    writerFloat4[0] = { SCREEN_SIZE_X / 2.0f,SCREEN_SIZE_Y / 2.0f,0.0f,SPHERE_SIZE };   //球
    writerFloat4[1] = { CAMERA_POS.x,CAMERA_POS.y,CAMERA_POS.z,0.0f };                  //カメラ

    //float
    int cBuffFloat = CreateShaderConstantBuffer(sizeof(float) * 8);
    float* writerFloat = static_cast<float*>(GetBufferShaderConstantBuffer(cBuffFloat));
    writerFloat[0] = 0.0f;              //カウント
    writerFloat[1] = PRIMITIVE_SIZE_X;  //解像度ｘ
    writerFloat[2] = PRIMITIVE_SIZE_Y;  //解像度ｙ
    writerFloat[3] = LOOP_NUM;          //ループ数
    writerFloat[4] = 1.85f;             //雲密度

    enum class STATE
    {
        Cloud,
        Sphere,
        Max
    };
    STATE state = STATE::Cloud;

    while (ProcessMessage() != -1)
    {
        SetDrawScreen(DX_SCREEN_BACK);
        ClearDrawScreen();

        //定数バッファ
        SetShaderConstantBuffer(cBuffFloat4, DX_SHADERTYPE_PIXEL, 5);
        SetShaderConstantBuffer(cBuffFloat, DX_SHADERTYPE_PIXEL, 6);
        UpdateShaderConstantBuffer(cBuffFloat4);
        UpdateShaderConstantBuffer(cBuffFloat);

        //描画開始
        if (state == STATE::Cloud)
        {
            DrawRayMarching(psCloud);
            DrawString(0, 0, "Cloud", 0xff2255);
        }
        else
        {
            DrawRayMarching(psSphere);
            DrawString(0, 0, "Sphere", 0xff2255);
        }

        //カウンター
        writerFloat[0] += 0.01f;

        //しきい値変更用
        if (CheckHitKey(KEY_INPUT_UP))
        {
            writerFloat[4] += 0.01f;
        }
        if (CheckHitKey(KEY_INPUT_DOWN))
        {
            writerFloat[4] -= 0.01f;
        }

        //描画しているオブジェクト変更
        if (CheckHitKey(KEY_INPUT_1))
        {
            state = STATE::Cloud;
        }
        if (CheckHitKey(KEY_INPUT_2))
        {
            state = STATE::Sphere;
        }

        ScreenFlip();
    }

    //破棄
    DeleteShader(psCloud);
    DeleteShaderConstantBuffer(cBuffFloat4);
    DeleteShaderConstantBuffer(cBuffFloat);

    DxLib_End();
    return 0;
}
