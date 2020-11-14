#include "stdafx.h"

// General
#include "PassForward_DoRenderScene.h"

// Additional
#include "Materials/MaterialModel.h"

CPassForward_DoRenderScene::CPassForward_DoRenderScene(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	
}

CPassForward_DoRenderScene::~CPassForward_DoRenderScene()
{}


IShaderParameter * CPassForward_DoRenderScene::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassForward_DoRenderScene::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	__super::ConfigurePipeline(RenderTarget, Viewport);

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Model_Forward.hlsl", "VS_PTNBB");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Forward.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	m_ShaderBonesBufferParameter = &vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

	m_ShaderLightsBufferParameter = &pixelShader->GetShaderParameterByName("LightsVS");
	//_ASSERT(m_ShaderLightsBufferParameter->IsValid());

	m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CPassForward_DoRenderScene::Visit(const ISceneNode * SceneNode)
{
	if (SceneNode->GetClass() == cSceneNode3D || SceneNode->GetClass() == cSceneNodeRTSUnit || SceneNode->GetClass() == cSceneNodeRTSBullet)
	{
		auto modelsComponent = SceneNode->GetComponentT<IModelsComponent3D>();
		if (modelsComponent != nullptr)
			if (m_ShaderBonesBufferParameter->IsValid())
				m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesBuffer());

		return Base3DPass::Visit(SceneNode);
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CPassForward_DoRenderScene::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CPassForward_DoRenderScene::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	objMaterial->Bind(GetRenderEventArgs().PipelineState->GetShaders());

	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Bind();
	if (m_ShaderLightsBufferParameter->IsValid())
		m_ShaderLightsBufferParameter->Bind();

	Geometry->Render(GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);

	if (m_ShaderLightsBufferParameter->IsValid())
		m_ShaderLightsBufferParameter->Unbind();
	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Unbind();

	objMaterial->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return AllowAll;
}
