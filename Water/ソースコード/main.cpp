#include<DxLib.h>
#include"Geometry.h"
#include <cmath>

//水のサイズ
constexpr float WATER_SCALE = 3.0f;

//モデルサイズ
constexpr float MODEL_SCALE = 1.0f;

//キューブマップのサイズ
constexpr int CUBE_MAP_SIZE = 1024;

//キューブマップの数
constexpr int CUBE_MAP_NUM = 6;

constexpr VECTOR CAMERA_INIT_POS = { 0, 400, -1500 };   //カメラ{ 0, 1600, -1000 }{ 0, 400, -400 }
constexpr VECTOR MODEL_INIT_POS = { 0, 100, 300 };      //反射モデル
constexpr VECTOR WATER_POS = { 0, 0, 0 };               //水
constexpr VECTOR VECTOR_ZERO = { 0, 0, 0 };             //ゼロ
constexpr VECTOR TAREGET_INIT_POS = { 0, 0, 0 };        //注視点

//カメラの移動速度
constexpr float MOVE_SPEED = 5.0f;

//波の移動速度
constexpr float ADD_WAVE_SPEED = 0.02f;

//モデルの描画
void DrawModelForMyShader(int model, int vs, int ps,int dynamicCube)
{
    //シェーダを有効にする
    MV1SetUseOrigShader(true);

    SetUseTextureToShader(0, dynamicCube);

    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    MV1DrawModel(model);

    //シェーダを無効にする
    MV1SetUseOrigShader(false);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    SetWindowText("water");
    ChangeWindowMode(true);
    DxLib_Init();
    SetBackgroundColor(0, 128, 255);

    //読み込み
    //モデル
    int waterModel = MV1LoadModel("model/water10.mv1");     //水
    int skyDomeModel = MV1LoadModel("model/dome/SkyDome.mv1");            //ドーム

    int vs = LoadVertexShader("shader/water_vs.vso"); //頂点
    int ps = LoadPixelShader("shader/water_ps.pso");  //ピクセル

    VECTOR cPos = CAMERA_INIT_POS;
    VECTOR mPos = MODEL_INIT_POS;
    VECTOR tPos = {};
    //定数バッファ
    //float
    int cbuff = CreateShaderConstantBuffer(sizeof(float) * 4);
    //渡す情報
    float* writer = static_cast<float*>(GetBufferShaderConstantBuffer(cbuff));
    //タイム:cnt
    writer[0] = 0.0f;
    //重力:gravity
    writer[1] = 9.8f;
    //π:pi
    writer[2] = DX_PI_F;

    //float4
    int cBuffFloat4 = CreateShaderConstantBuffer(sizeof(FLOAT4) * 4);
    //渡す情報
    FLOAT4* writerFloat4 = static_cast<FLOAT4*>(GetBufferShaderConstantBuffer(cBuffFloat4));
    //カメラ位置
    writerFloat4[0] = { cPos.x, cPos.y, cPos.z,0.0f };

    //環境を描画する際に使用するカメラの注視点と上方向
    //注視点
    VECTOR targetPos[CUBE_MAP_NUM] = {};
    targetPos[0] = VGet(1.0f, 0.0f, 0.0f);
    targetPos[1] = VGet(-1.0f, 0.0f, 0.0f);
    targetPos[2] = VGet(0.0f, 1.0f, 0.0f);
    targetPos[3] = VGet(0.0f, -1.0f, 0.0f);
    targetPos[4] = VGet(0.0f, 0.0f, 1.0f);
    targetPos[5] = VGet(0.0f, 0.0f, -1.0f);

    //上方向
    VECTOR cameraUp[CUBE_MAP_NUM] = {};
    cameraUp[0] = VGet(0.0f, 1.0f, 0.0f);
    cameraUp[1] = VGet(0.0f, 1.0f, 0.0f);
    cameraUp[2] = VGet(0.0f, 0.0f, -1.0f);
    cameraUp[3] = VGet(0.0f, 0.0f, 1.0f);
    cameraUp[4] = VGet(0.0f, 1.0f, 0.0f);
    cameraUp[5] = VGet(0.0f, 1.0f, 0.0f);

    SetCameraPositionAndTarget_UpVecY(cPos, TAREGET_INIT_POS);
    //配置
    MV1SetPosition(waterModel, WATER_POS);

    //スケール
    MV1SetScale(waterModel, VGet(WATER_SCALE, WATER_SCALE, WATER_SCALE));
    MV1SetScale(skyDomeModel, VGet(30, 30, 30));

    //キューブマップテクスチャ
    SetCubeMapTextureCreateFlag(true);
    int dynamicCubeTexture = MakeScreen(CUBE_MAP_SIZE, CUBE_MAP_SIZE, true);
    SetCubeMapTextureCreateFlag(false);

    while (ProcessMessage() != -1)
    {
        //環境を描画する面の数
        for (int i = 0; i < CUBE_MAP_NUM; i++)
        {
            //描画先をレンダーターゲットに設定
            SetRenderTargetToShader(0, dynamicCubeTexture, i);
            ClearDrawScreen();

            //カメラの画角は90度
            SetupCamera_Perspective(90.0f / 180.0f * DX_PI_F);

            //クリップ
            SetCameraNearFar(1.0f, 10000.0f);

            //カメラの設定
            SetCameraPositionAndTargetAndUpVec(WATER_POS, targetPos[i], cameraUp[i]);

            //モデル描画
            MV1DrawModel(skyDomeModel);
        }

        //裏画面描画
        SetDrawScreen(DX_SCREEN_BACK);
        ClearDrawScreen();

        //カメラ設定
        SetupCamera_Perspective(60.0f / 180.0f * DX_PI_F);
        SetCameraPositionAndTarget_UpVecY(cPos, tPos);

        //モデルの描画
        MV1DrawModel(skyDomeModel);

        //水描画
        DrawModelForMyShader(waterModel, vs, ps, dynamicCubeTexture);

        //定数
        //float型
        SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_VERTEX, 5);
        SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 5);
        UpdateShaderConstantBuffer(cbuff);
        //float4型
        SetShaderConstantBuffer(cBuffFloat4, DX_SHADERTYPE_VERTEX, 6);
        SetShaderConstantBuffer(cBuffFloat4, DX_SHADERTYPE_PIXEL, 6);
        UpdateShaderConstantBuffer(cBuffFloat4);

        //カメラの位置を移動
        if (CheckHitKey(KEY_INPUT_UP))
        {
            cPos.y += MOVE_SPEED;
        }
        if (CheckHitKey(KEY_INPUT_DOWN))
        {
            cPos.y -= MOVE_SPEED;
        }

        writer[0] += ADD_WAVE_SPEED;
        writerFloat4[0] = { cPos.x, cPos.y, cPos.z,0 };
        ScreenFlip();
    }

    //破棄
    DeleteShader(ps);
    DeleteShader(vs);

    DeleteShaderConstantBuffer(cbuff);
    DeleteShaderConstantBuffer(cBuffFloat4);

    DeleteGraph(dynamicCubeTexture);

    MV1DeleteModel(waterModel);
    MV1DeleteModel(skyDomeModel);

    DxLib_End();
    return 0;
}
