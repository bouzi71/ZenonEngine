#include "stdafx.h"

// General
#include "PassDeffered_RenderUIQuad.h"

CPassDeffered_RenderUIQuad::CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, std::shared_ptr<CPassDeffered_DoRenderScene> DefferedRender, std::shared_ptr<CPassDeffered_ProcessLights> DefferedRenderPrepareLights)
	: RenderPassPipelined(RenderDevice)
	, m_DefferedRender(DefferedRender)
	, m_DefferedRenderPrepareLights(DefferedRenderPrepareLights)
{
	m_LightResultData = (SLightResult*)_aligned_malloc(sizeof(SLightResult), 16);
	m_LightResultConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SLightResult());
}

CPassDeffered_RenderUIQuad::~CPassDeffered_RenderUIQuad()
{
	_aligned_free(m_LightResultData);
}



//
// IRenderPass
//

void CPassDeffered_RenderUIQuad::Render(RenderEventArgs& e)
{
	for (const auto& lightResult : m_DefferedRenderPrepareLights->GetLightResult())
	{
		// Once per light
		BindLightParamsForCurrentIteration(e, lightResult);

		m_QuadGeometry->Render(e, GetPipeline().GetShaders().at(EShaderType::VertexShader).get());
	}
}

void CPassDeffered_RenderUIQuad::PostRender(RenderEventArgs& e)
{
	RenderPassPipelined::PostRender(e);
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDeffered_RenderUIQuad::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	auto vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "3D/Deffered_UIQuad.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", "1" } });
	vertexShader->LoadInputLayoutFromReflector();

	auto pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "3D/Deffered_UIQuad.hlsl", "PS_DeferredLighting", { {"MULTISAMPLED", "1" }});

	// PIPELINES
	auto defferedFinalPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	defferedFinalPipeline->GetBlendState()->SetBlendMode(additiveBlending);
	defferedFinalPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	defferedFinalPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Front);
	defferedFinalPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	//defferedFinalPipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
	//defferedFinalPipeline->GetRasterizerState()->SetMultisampleEnabled(true);
	defferedFinalPipeline->SetRenderTarget(RenderTarget);
	defferedFinalPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	defferedFinalPipeline->SetShader(EShaderType::PixelShader, pixelShader);
	

	auto& sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	defferedFinalPipeline->SetSampler(0, sampler);

	auto& samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	defferedFinalPipeline->SetSampler(1, samplerClamp);

	defferedFinalPipeline->SetTexture(0, m_DefferedRender->GetTexture0());
	defferedFinalPipeline->SetTexture(1, m_DefferedRender->GetTexture1());
	defferedFinalPipeline->SetTexture(2, m_DefferedRender->GetTexture2());
	defferedFinalPipeline->SetTexture(3, m_DefferedRender->GetTexture3());
	defferedFinalPipeline->SetTexture(4, m_DefferedRender->GetTextureDepthStencil());

	return SetPipeline(defferedFinalPipeline);
}



//
// Protected
//
void CPassDeffered_RenderUIQuad::BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CPassDeffered_ProcessLights::SLightResult& LightResult)
{
	const ICameraComponent3D* camera = e.Camera;
	_ASSERT(camera != nullptr);

	SLightResult lightResult;
	lightResult.Light = LightResult.Light->GetLightStruct();
	lightResult.Light.PositionVS = camera->GetViewMatrix() * glm::vec4(lightResult.Light.PositionWS.xyz(), 1.0f);
	lightResult.Light.DirectionVS = glm::normalize(camera->GetViewMatrix() * glm::vec4(lightResult.Light.DirectionWS.xyz(), 0.0f));
	lightResult.LightViewMatrix = LightResult.Light->GetViewMatrix();
	lightResult.LightProjectionMatrix = LightResult.Light->GetProjectionMatrix();
	lightResult.IsShadowEnabled = LightResult.IsShadowEnable;

	{
	    *m_LightResultData = lightResult;
		m_LightResultConstantBuffer->Set(*m_LightResultData);

		auto& lightParam = GetPipeline().GetShader(EShaderType::PixelShader)->GetShaderParameterByName("LightResult");
		if (lightParam.IsValid() && m_LightResultConstantBuffer != nullptr)
		{
			lightParam.SetConstantBuffer(m_LightResultConstantBuffer);
			lightParam.Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}

	{
		auto& shadowTexture = GetPipeline().GetShader(EShaderType::PixelShader)->GetShaderParameterByName("TextureShadow");
		if (shadowTexture.IsValid() && LightResult.IsShadowEnable && LightResult.ShadowTexture != nullptr)
		{
			shadowTexture.SetTexture(LightResult.ShadowTexture);
			shadowTexture.Bind();
		}
		else
		{
			//_ASSERT(false);
		}
	}
}
