#include<DxLib.h>
#include"Geometry.h"
#include <array>

//�X�N���[���T�C�Y
constexpr float SCREEN_SIZE_X = 640.0f; //x
constexpr float SCREEN_SIZE_Y = 480.0f; //y

//�v���~�e�B�u�T�C�Y
constexpr float PRIMITIVE_SIZE_X = 640.0f;  //x
constexpr float PRIMITIVE_SIZE_Y = 480.0f;  //y
constexpr float PRIMITIVE_SIZE_Z = 400;  //z

//�J�����ʒu
constexpr VECTOR CAMERA_POS = {0.0f,0.0f, -10.0f };

//�����
constexpr float SPHERE_SIZE = 0.5f;

//���[�v��
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

    //UV���W
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
    SetWindowText("�_");
    ChangeWindowMode(true);
    SetBackgroundColor(0, 156, 255);
    DxLib_Init();
    SetDrawScreen(DX_SCREEN_BACK);

    int psCloud = LoadPixelShader("Cloud_ps.pso");          //�_�p
    int psSphere = LoadPixelShader("Sphere_ps.pso");   //���p

    //�萔�o�b�t�@
    int cBuffFloat4 = CreateShaderConstantBuffer(sizeof(FLOAT4) * 4);
    FLOAT4* writerFloat4 = static_cast<FLOAT4*>(GetBufferShaderConstantBuffer(cBuffFloat4));

    writerFloat4[0] = { SCREEN_SIZE_X / 2.0f,SCREEN_SIZE_Y / 2.0f,0.0f,SPHERE_SIZE };   //��
    writerFloat4[1] = { CAMERA_POS.x,CAMERA_POS.y,CAMERA_POS.z,0.0f };                  //�J����

    //float
    int cBuffFloat = CreateShaderConstantBuffer(sizeof(float) * 8);
    float* writerFloat = static_cast<float*>(GetBufferShaderConstantBuffer(cBuffFloat));
    writerFloat[0] = 0.0f;              //�J�E���g
    writerFloat[1] = PRIMITIVE_SIZE_X;  //�𑜓x��
    writerFloat[2] = PRIMITIVE_SIZE_Y;  //�𑜓x��
    writerFloat[3] = LOOP_NUM;          //���[�v��
    writerFloat[4] = 1.85f;             //�_���x

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

        //�萔�o�b�t�@
        SetShaderConstantBuffer(cBuffFloat4, DX_SHADERTYPE_PIXEL, 5);
        SetShaderConstantBuffer(cBuffFloat, DX_SHADERTYPE_PIXEL, 6);
        UpdateShaderConstantBuffer(cBuffFloat4);
        UpdateShaderConstantBuffer(cBuffFloat);

        //�`��J�n
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

        //�J�E���^�[
        writerFloat[0] += 0.01f;

        //�������l�ύX�p
        if (CheckHitKey(KEY_INPUT_UP))
        {
            writerFloat[4] += 0.01f;
        }
        if (CheckHitKey(KEY_INPUT_DOWN))
        {
            writerFloat[4] -= 0.01f;
        }

        //�`�悵�Ă���I�u�W�F�N�g�ύX
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

    //�j��
    DeleteShader(psCloud);
    DeleteShaderConstantBuffer(cBuffFloat4);
    DeleteShaderConstantBuffer(cBuffFloat);

    DxLib_End();
    return 0;
}
