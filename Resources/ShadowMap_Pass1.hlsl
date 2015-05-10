//------------------------------------------------
//Shadow Map Pass0
//------------------------------------------------


cbuffer CBuffer : register(b0)
{
	column_major float4x4 g_matCWVP		: packoffset(c0);
	column_major float4x4 g_matLWVP		: packoffset(c4);
	float4 g_vecLightPos				: packoffset(c8);
	float4 g_DepthMapSize				: packoffset(c9);
};

struct VS_IN
{
	float3 pos		: POSITION;
	float3 normal	: NORMAL;
	float2 texel	: TEXCOORD;
};

Texture2D g_DecalMap : register(t0);

Texture2D g_DepthMap : register(t1);

SamplerState g_Sampler : register(s0);

struct VS_OUT_PS_IN
{
	float4 pos		: SV_POSITION;
	float3 normal	: NORMAL;
	float2 texel	: TEXCOORD0;
	float4 posLWVP	: TEXCOORD1;
};

VS_OUT_PS_IN ShadowMap_Pass1_VS_Main(VS_IN In)
{
	VS_OUT_PS_IN Out;

	Out.pos = mul(float4(In.pos, 1.0f), g_matCWVP);
	Out.posLWVP = mul(float4(In.pos, 1.0f), g_matLWVP);
	Out.normal = In.normal;
	Out.texel = In.texel;

	return Out;

}

float4 ShadowMap_Pass1_PS_Main(VS_OUT_PS_IN In) : SV_TARGET
{
	float shadowColor;

	float2 texel = float2(In.posLWVP.x / In.posLWVP.w * 0.5f + 0.5f, In.posLWVP.y / In.posLWVP.w * -0.5f + 0.5f);

		//深度マップの範囲外は影なしにする
		if (texel.x < 0 || texel.x > 1 || texel.y < 0 || texel.y > 1)
			shadowColor = 1.0f;
		else
		{
			float sm = g_DepthMap.Load(float3(texel * g_DepthMapSize.xy, 0)).r;

			if (sm < In.posLWVP.z / In.posLWVP.w - 0.002f)
				shadowColor = 0.3f;
			else
				shadowColor = 1.0f;
		}

	float lambert = dot(g_vecLightPos.xyz, In.normal);

	lambert = lambert * 0.5f + 0.5f;

	float4 decalmap = g_DecalMap.Sample(g_Sampler, In.texel);

	return (decalmap * min(lambert, shadowColor)).rgba;
}