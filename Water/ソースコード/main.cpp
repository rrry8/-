#include<DxLib.h>
#include"Geometry.h"
#include <cmath>

//���̃T�C�Y
constexpr float WATER_SCALE = 3.0f;

//���f���T�C�Y
constexpr float MODEL_SCALE = 1.0f;

//�L���[�u�}�b�v�̃T�C�Y
constexpr int CUBE_MAP_SIZE = 1024;

//�L���[�u�}�b�v�̐�
constexpr int CUBE_MAP_NUM = 6;

constexpr VECTOR CAMERA_INIT_POS = { 0, 400, -1500 };   //�J����{ 0, 1600, -1000 }{ 0, 400, -400 }
constexpr VECTOR MODEL_INIT_POS = { 0, 100, 300 };      //���˃��f��
constexpr VECTOR WATER_POS = { 0, 0, 0 };               //��
constexpr VECTOR VECTOR_ZERO = { 0, 0, 0 };             //�[��
constexpr VECTOR TAREGET_INIT_POS = { 0, 0, 0 };        //�����_

//�J�����̈ړ����x
constexpr float MOVE_SPEED = 5.0f;

//�g�̈ړ����x
constexpr float ADD_WAVE_SPEED = 0.02f;

//���f���̕`��
void DrawModelForMyShader(int model, int vs, int ps,int dynamicCube)
{
    //�V�F�[�_��L���ɂ���
    MV1SetUseOrigShader(true);

    SetUseTextureToShader(0, dynamicCube);

    SetUseVertexShader(vs);
    SetUsePixelShader(ps);
    MV1DrawModel(model);

    //�V�F�[�_�𖳌��ɂ���
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

    //�ǂݍ���
    //���f��
    int waterModel = MV1LoadModel("model/water10.mv1");     //��
    int skyDomeModel = MV1LoadModel("model/dome/SkyDome.mv1");            //�h�[��

    int vs = LoadVertexShader("shader/water_vs.vso"); //���_
    int ps = LoadPixelShader("shader/water_ps.pso");  //�s�N�Z��

    VECTOR cPos = CAMERA_INIT_POS;
    VECTOR mPos = MODEL_INIT_POS;
    VECTOR tPos = {};
    //�萔�o�b�t�@
    //float
    int cbuff = CreateShaderConstantBuffer(sizeof(float) * 4);
    //�n�����
    float* writer = static_cast<float*>(GetBufferShaderConstantBuffer(cbuff));
    //�^�C��:cnt
    writer[0] = 0.0f;
    //�d��:gravity
    writer[1] = 9.8f;
    //��:pi
    writer[2] = DX_PI_F;

    //float4
    int cBuffFloat4 = CreateShaderConstantBuffer(sizeof(FLOAT4) * 4);
    //�n�����
    FLOAT4* writerFloat4 = static_cast<FLOAT4*>(GetBufferShaderConstantBuffer(cBuffFloat4));
    //�J�����ʒu
    writerFloat4[0] = { cPos.x, cPos.y, cPos.z,0.0f };

    //����`�悷��ۂɎg�p����J�����̒����_�Ə����
    //�����_
    VECTOR targetPos[CUBE_MAP_NUM] = {};
    targetPos[0] = VGet(1.0f, 0.0f, 0.0f);
    targetPos[1] = VGet(-1.0f, 0.0f, 0.0f);
    targetPos[2] = VGet(0.0f, 1.0f, 0.0f);
    targetPos[3] = VGet(0.0f, -1.0f, 0.0f);
    targetPos[4] = VGet(0.0f, 0.0f, 1.0f);
    targetPos[5] = VGet(0.0f, 0.0f, -1.0f);

    //�����
    VECTOR cameraUp[CUBE_MAP_NUM] = {};
    cameraUp[0] = VGet(0.0f, 1.0f, 0.0f);
    cameraUp[1] = VGet(0.0f, 1.0f, 0.0f);
    cameraUp[2] = VGet(0.0f, 0.0f, -1.0f);
    cameraUp[3] = VGet(0.0f, 0.0f, 1.0f);
    cameraUp[4] = VGet(0.0f, 1.0f, 0.0f);
    cameraUp[5] = VGet(0.0f, 1.0f, 0.0f);

    SetCameraPositionAndTarget_UpVecY(cPos, TAREGET_INIT_POS);
    //�z�u
    MV1SetPosition(waterModel, WATER_POS);

    //�X�P�[��
    MV1SetScale(waterModel, VGet(WATER_SCALE, WATER_SCALE, WATER_SCALE));
    MV1SetScale(skyDomeModel, VGet(30, 30, 30));

    //�L���[�u�}�b�v�e�N�X�`��
    SetCubeMapTextureCreateFlag(true);
    int dynamicCubeTexture = MakeScreen(CUBE_MAP_SIZE, CUBE_MAP_SIZE, true);
    SetCubeMapTextureCreateFlag(false);

    while (ProcessMessage() != -1)
    {
        //����`�悷��ʂ̐�
        for (int i = 0; i < CUBE_MAP_NUM; i++)
        {
            //�`���������_�[�^�[�Q�b�g�ɐݒ�
            SetRenderTargetToShader(0, dynamicCubeTexture, i);
            ClearDrawScreen();

            //�J�����̉�p��90�x
            SetupCamera_Perspective(90.0f / 180.0f * DX_PI_F);

            //�N���b�v
            SetCameraNearFar(1.0f, 10000.0f);

            //�J�����̐ݒ�
            SetCameraPositionAndTargetAndUpVec(WATER_POS, targetPos[i], cameraUp[i]);

            //���f���`��
            MV1DrawModel(skyDomeModel);
        }

        //����ʕ`��
        SetDrawScreen(DX_SCREEN_BACK);
        ClearDrawScreen();

        //�J�����ݒ�
        SetupCamera_Perspective(60.0f / 180.0f * DX_PI_F);
        SetCameraPositionAndTarget_UpVecY(cPos, tPos);

        //���f���̕`��
        MV1DrawModel(skyDomeModel);

        //���`��
        DrawModelForMyShader(waterModel, vs, ps, dynamicCubeTexture);

        //�萔
        //float�^
        SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_VERTEX, 5);
        SetShaderConstantBuffer(cbuff, DX_SHADERTYPE_PIXEL, 5);
        UpdateShaderConstantBuffer(cbuff);
        //float4�^
        SetShaderConstantBuffer(cBuffFloat4, DX_SHADERTYPE_VERTEX, 6);
        SetShaderConstantBuffer(cBuffFloat4, DX_SHADERTYPE_PIXEL, 6);
        UpdateShaderConstantBuffer(cBuffFloat4);

        //�J�����̈ʒu���ړ�
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

    //�j��
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
