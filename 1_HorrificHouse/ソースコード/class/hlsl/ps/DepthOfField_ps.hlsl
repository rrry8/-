struct PS_INPUT
{
    float4 pos : POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
};

cbuffer BaseCBuffer : register(b1)
{
    matrix AntiViewportM; //4x4ビューポート逆行列
    matrix ProjectionM; //4x4プロジェクション行列
    float4x3 ViewM; //4x3(ビュー行列)
    float4x3 LocalM; //4x3(回転拡縮平行移動)
    float4 ToonOutLineSize; // トゥーンの輪郭線の大きさ
    float DiffuseSource; // ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
    float SpecularSource; // スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
    float MulSpecularColor; // スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
    float Padding; //詰め物(無視)
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
    float focusStartPos;    //x:フォーカス開始位置
    float focusEndPos;      //y:フォーカス終了位置
}

SamplerState sam:register(s0);
Texture2D screenTex : register(t0);     //スクリーン
Texture2D blurTex : register(t1);       //ぼかし
Texture2D highBLurTex : register(t2);   //強ぼかし
Texture2D depthTex : register(t3);      //深度

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    //深度値をサンプリング
    float depth = screenTex.Sample(sam, input.uv).x;
    float4 finalColor;
    float4 color = screenTex.Sample(sam, input.uv);

    float4 color1, color2;
    float blendRate;
    float fade;

    //輝度が高い場合はそのまま描画する
    float luminance = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    if (luminance - 0.4f < 0.0f)
    {
        //フォーカス位置からぼかし率を計算
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

        //ぼかし率から計算
        if (fade < 0.5f)
        {
            //低い場合は弱めのぼかした画像と合成
            color1 = screenTex.Sample(sam, input.uv);
            color2 = blurTex.Sample(sam, input.uv);
            blendRate = fade * 2.0f;
        }
        else
        {
            //高い場合は強めのぼかした画像と合成
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
