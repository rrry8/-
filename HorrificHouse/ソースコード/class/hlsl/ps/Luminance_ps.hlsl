// ピクセルシェーダーの入力
struct PS_INPUT
{
	float4 svpos:SV_POSITION;
	float4 diffuse: COLOR;
	float2 uv : TEXCOORD;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
	float4 color           : SV_TARGET0;	// 色
};

SamplerState sam            : register(s0);		//サンプラ
Texture2D    tex            : register(t0);		//スクリーン

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;
	float4 color = tex.Sample(sam, input.uv);

	//輝度計算
	float luminance = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));

	//輝度が低いものは描画しない
	clip(luminance - 0.95f);

	output.color = color;

	return output;
}
