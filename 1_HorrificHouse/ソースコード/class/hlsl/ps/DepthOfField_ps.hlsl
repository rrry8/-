struct PS_INPUT
{
    float4 pos : POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer BaseCBuffer : register(b1)
{
    matrix AntiViewportM; //4x4�r���[�|�[�g�t�s��
    matrix ProjectionM; //4x4�v���W�F�N�V�����s��
    float4x3 ViewM; //4x3(�r���[�s��)
    float4x3 LocalM; //4x3(��]�g�k���s�ړ�)
    float4 ToonOutLineSize; // �g�D�[���̗֊s���̑傫��
    float DiffuseSource; // �f�B�t���[�Y�J���[( 0.0f:�}�e���A��  1.0f:���_ )
    float SpecularSource; // �X�y�L�����J���[(   0.0f:�}�e���A��  1.0f:���_ )
    float MulSpecularColor; // �X�y�L�����J���[�l�ɏ�Z����l( �X�y�L�������������Ŏg�p )
    float Padding; //�l�ߕ�(����)
}

struct PS_OUTPUT
{
    float4 color : SV_TARGET;
};

cbuffer DirectionLightBuffer : register(b9)
{
    float4 pad;
    float4 pad2;
    float4 pad3;
    float focusStartPos;    //x:�t�H�[�J�X�J�n�ʒu
    float focusEndPos;      //y:�t�H�[�J�X�I���ʒu
}

SamplerState sam:register(s0);
Texture2D screenTex : register(t0);     //�X�N���[��
Texture2D blurTex : register(t1);       //�ڂ���
Texture2D highBLurTex : register(t2);   //���ڂ���
Texture2D depthTex : register(t3);      //�[�x

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    //�[�x�l���T���v�����O
    float depth = screenTex.Sample(sam, input.uv).x;
    float4 finalColor;
    float4 color = screenTex.Sample(sam, input.uv);

    float4 color1, color2;
    float blendRate;
    float fade;

    //�P�x�������ꍇ�͂��̂܂ܕ`�悷��
    float luminance = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    if (luminance - 0.4f < 0.0f)
    {
        //�t�H�[�J�X�ʒu����ڂ��������v�Z
        if (depth < focusStartPos)
        {
            fade = 1.0f - depth / focusStartPos;
        }
        else if (depth < focusEndPos)
        {
            fade = 0.0f;
        }
        else
        {
            fade = (depth - focusEndPos) / (1.0f - focusEndPos);
        }

        //�ڂ���������v�Z
        if (fade < 0.5f)
        {
            //�Ⴂ�ꍇ�͎�߂̂ڂ������摜�ƍ���
            color1 = screenTex.Sample(sam, input.uv);
            color2 = blurTex.Sample(sam, input.uv);
            blendRate = fade * 2.0f;
        }
        else
        {
            //�����ꍇ�͋��߂̂ڂ������摜�ƍ���
            color1 = blurTex.Sample(sam, input.uv);
            color2 = highBLurTex.Sample(sam, input.uv);
            blendRate = (fade - 0.5f) * 2.0f;
        }
        finalColor = lerp(color1, color2, blendRate);
    }
    else
    {
        finalColor = color;
    }
    output.color = finalColor;
    return output;
}
