//------------------------------------------------
//Shadow Map Pass0
//------------------------------------------------

//定数バッファ
cbuffer CBuffer : register (b0)
{
	column_major float4x4 g_matLWVP		: packoffset(c0); //ライトビューの正射影行列
};

struct VS_IN
{
	float3 pos		: POSITION;
	float3 normal	: NORMAL;
	float2 texel	: TEXCOORD;
};

struct VS_OUT_PS_IN
{
	float4 pos		: SV_POSITION;
};


VS_OUT_PS_IN ShadowMap_Pass0_VS_Main(VS_IN In)
{
	VS_OUT_PS_IN Out;

	Out.pos = mul(float4(In.pos, 1.0f), g_matLWVP);

	return Out;
}
