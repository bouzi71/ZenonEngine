#include "stdafx.h"

// General
#include "TextureMaterialPass.h"

// Additional
#include "Materials/MaterialTextured.h"

CTexturedMaterialPass::CTexturedMaterialPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CTexturedMaterialPass::~CTexturedMaterialPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CTexturedMaterialPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_TEXTURED", "VS_main");
		g_pPixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_TEXTURED", "PS_main");
	}
	else
	{
		g_pVertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_OGL_3D_TEXTURED_VS", "");
		g_pPixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_OGL_3D_TEXTURED_PS", "");
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	//std::vector<SCustomVertexElement> elements;
	//elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	//elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	//elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	//g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	// PIPELINES
	auto Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState()->SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, g_pVertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, g_pPixelShader);

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	Pipeline->SetSampler(0, sampler);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
bool CTexturedMaterialPass::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

bool CTexturedMaterialPass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialTextured* objMaterial = dynamic_cast<const MaterialTextured*>(Material);
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}