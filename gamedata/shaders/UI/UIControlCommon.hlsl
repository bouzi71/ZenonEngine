#include "UI/UI_VertexBase.hlsl"

cbuffer Material : register(b2)
{
    float4 DiffuseColor;
	bool HasDiffuseTexture;
};

cbuffer Subgeometry : register(b3)
{
    float2 Translate;
	float2 Size;
};

Texture2D DiffuseTexture : register(t0);

float4 PS_main(VSOutputUI IN) : SV_TARGET
{
	float4 resultColor = DiffuseColor;
	if (HasDiffuseTexture)
		resultColor *= DiffuseTexture.Sample(LinearClampSampler, float2(IN.texCoord.x, IN.texCoord.y));
	
    return resultColor;
}