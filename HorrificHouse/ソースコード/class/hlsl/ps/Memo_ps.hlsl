//入力
struct PS_INPUT {
	float4 pos    : SV_POSITION;
	float4 diffuse     : COLOR0;
	float2 uv  : TEXCOORD0;
};
//出力
struct PS_OUTPUT
{
	float4 color      : SV_TARGET;
};

//テクスチャ
SamplerState sam     : register(s0);
Texture2D    screen  : register(t0);

PS_OUTPUT main(PS_INPUT input)
{
	PS_OUTPUT output;

	float4 color = screen.Sample(sam,input.uv);

	//写真の枠を作成する
	float offset = 1080.0f / 1920.0f;
	if ((0.05f * offset > input.uv.x || input.uv.x > 1.0f - 0.05f * offset) ||
		(0.05f > input.uv.y || input.uv.y > 0.95f))
	{
		color.rgb = input.diffuse.rgb;
	}


	output.color = color;

	return output;
}
