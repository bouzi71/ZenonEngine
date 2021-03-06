#include "CommonInclude.hlsl"

cbuffer Material : register(b2)
{
    float4 Color;
	bool IsSelected;
	float3 __padding1;
};

cbuffer PerCharacterDataVS : register(b3)
{
	float2 Offset;
	float2 __padding0;
};

Texture2D DiffuseTexture : register(t0);

VSOutputUI VS_main(VSInputUI IN)
{
	const float4 resultPos = float4(IN.position, 0.0f, 1.0f) + float4(Offset, 0.0f, 0.0f);
	
	const float4x4 mp = mul(PF.Projection, PO.Model);

	VSOutputUI OUT;
	OUT.position = mul(mp, resultPos);
	OUT.texCoord = IN.texCoord;
	return OUT;
}

float4 PS_main(VSOutputUI IN) : SV_TARGET
{
	const float fontAlpha = DiffuseTexture.Sample(LinearClampSampler, IN.texCoord).a;

	float4 diffuseColor = float4(Color.rgb, Color.a * fontAlpha);
	if (IsSelected)
		diffuseColor = float4(/*float3(1.0f, 1.0f, 1.0f) - diffuseColor.rgb*/0, 0, 0 , (1.0f - fontAlpha));

    return diffuseColor;
}