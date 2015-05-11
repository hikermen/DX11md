// 定数バッファ０
cbuffer cbBuffer0 : register(b0)
{
	// 列優先
	column_major float4x4 g_matWVP : packoffset(c0);   // ワールド × ビュー × 射影 行列
};

// 定数バッファ１
cbuffer cbBuffer1 : register(b1)
{
	float g_TessFactor : packoffset(c0.x);   // パッチのエッジのテッセレーション係数
	float g_InsideTessFactor : packoffset(c0.y);   // パッチ内部のテッセレーション係数
};

// テクスチャー
Texture2D g_Tex : register(t0);

// サンプラーステート
SamplerState  g_Sampler;

// 頂点シェーダーの入力パラメータ
struct VS_IN
{
	float3 pos   : POSITION;   // 頂点座標
	float2 texel : TEXCOORD0;  // テクセル
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT
{
	float3 pos    : POSITION;
	float2 texel  : TEXCOORD0;
};

// ハルシェーダーのパッチ定数フェーズ用の出力パラメータ
struct CONSTANT_HS_OUT
{
	float Edges[3] : SV_TessFactor;        // パッチのエッジのテッセレーション係数
	float Inside : SV_InsideTessFactor; // パッチ内部のテッセレーション係数
};

// ハルシェーダーのコントロール ポイント フェーズ用の出力パラメータ
struct HS_OUT
{
	float3 pos   : POSITION;
	float2 texel : TEXCOORD0;
};

// ドメインシェーダーの出力パラメータ
struct DS_OUT
{
	float4 pos   : SV_POSITION;
	float2 texel : TEXCOORD0;
};

// *****************************************************************************************
// 頂点シェーダー
// *****************************************************************************************
VS_OUT VS_Main(VS_IN In)
{
	VS_OUT Out;

	// 頂点シェーダーではそのまま渡す
	Out.pos = In.pos;
	Out.texel = In.texel;
	return Out;
}

// *****************************************************************************************
// ハルシェーダーのパッチ定数フェーズ
// *****************************************************************************************
CONSTANT_HS_OUT ConstantsHS_Main(InputPatch<VS_OUT, 3> ip, uint PatchID : SV_PrimitiveID)
{
	CONSTANT_HS_OUT Out;

	// 定数バッファの値をそのまま渡す
	Out.Edges[0] = Out.Edges[1] = Out.Edges[2] = g_TessFactor;  // パッチのエッジのテッセレーション係数
	Out.Inside = g_InsideTessFactor;  // パッチ内部のテッセレーション係数

	return Out;
}

// *****************************************************************************************
// ハルシェーダーのコントロール ポイント フェーズ
// *****************************************************************************************
[domain("tri")]                             // テッセレートするメッシュの形状を指定する
[partitioning("integer")]                    // パッチの分割に使用するアルゴリズムを指定する
[outputtopology("triangle_ccw")]             // メッシュの出力方法を指定する
[outputcontrolpoints(3)]                     // ハルシェーダーのコントロール ポイント フェーズがコールされる回数
[patchconstantfunc("ConstantsHS_Main")]      // 対応するハルシェーダーのパッチ定数フェーズのメイン関数
HS_OUT HS_Main(InputPatch<VS_OUT, 3> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
	HS_OUT Out;

	// そのまま渡す
	Out.pos = In[i].pos;
	Out.texel = In[i].texel;
	return Out;
}

// *****************************************************************************************
// ドメインシェーダー
// *****************************************************************************************
[domain("tri")]
DS_OUT DS_Main(CONSTANT_HS_OUT In, float3 uvw : SV_DomainLocation, const OutputPatch<HS_OUT, 3> patch)
{
	DS_OUT Out;

	// 頂点座標
	float3 p1 = patch[2].pos * uvw.x + patch[1].pos * uvw.y + patch[0].pos * uvw.z;
		Out.pos = mul(float4(p1, 1.0f), g_matWVP);

	// テクセル
	float2 t1 = patch[2].texel * uvw.x + patch[1].texel * uvw.y + patch[0].texel * uvw.z;
		Out.texel = t1;

	return Out;
}

// *****************************************************************************************
// ピクセルシェーダ
// *****************************************************************************************
float4 PS_Main(DS_OUT In) : SV_TARGET
{
	return g_Tex.Sample(g_Sampler, In.texel);
}