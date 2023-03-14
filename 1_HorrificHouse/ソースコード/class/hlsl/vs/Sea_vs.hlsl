struct VS_INPUT
{
	float3 pos : POSITION;//座標
	float3 norm : NORMAL;//法線
	float4 diffuse : COLOR0;
	float4 specular : COLOR1;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float3 tangent:TANGENT;//接ベクトル
	float3 binormal:BINORMAL;//従法線ベクトル
};

struct VS_OUTPUT
{
	float4 svpos:SV_POSITION;
	float3 pos:POSITION0;
	float4 diffuse:COLOR;
	float3 normal:NORMAL;
	float2 uv:TECOORD0;
	float3 viewVec : TEXCOORD1;
};

cbuffer BaseCBuffer : register(b1)
{
	matrix AntiViewportM;//4x4ビューポート逆行列
	matrix ProjectionM;//4x4プロジェクション行列
	float4x3 ViewM;//4x3(ビュー行列)
	float4x3 LocalM;//4x3(回転拡縮平行移動)
	float4		ToonOutLineSize;						// トゥーンの輪郭線の大きさ
	float		DiffuseSource;							// ディフューズカラー( 0.0f:マテリアル  1.0f:頂点 )
	float		SpecularSource;						// スペキュラカラー(   0.0f:マテリアル  1.0f:頂点 )
	float		MulSpecularColor;						// スペキュラカラー値に乗算する値( スペキュラ無効処理で使用 )
	float		Padding;//詰め物(無視)
}

cbuffer cBuff : register(b5)
{
	float cnt;			//タイム
	float gravity;		//重力
	float pi;			//π
}

cbuffer cBuffFloat4 : register(b6)
{
	float4 cPos;	//カメラ位置(ワールド)
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT ret;
	float4 pos = float4(0.0f, 0.0f, 0.0f, 1.0f);

	pos.xyz += input.pos;

	pos.xyz = mul(pos, LocalM);//ワールド
	ret.pos = pos;
	pos.xyz = mul(pos, ViewM);//ビュー
	pos = mul(pos, ProjectionM);//プロジェクション

	ret.svpos = pos;

	ret.uv = input.uv0;
	ret.normal = normalize(input.norm);
	ret.diffuse = input.diffuse;
	ret.viewVec = cPos - ret.pos.xyz;

	return ret;
}