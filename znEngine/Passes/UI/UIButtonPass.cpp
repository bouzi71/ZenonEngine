#include "stdafx.h"

// General
#include "UIButtonPass.h"

// Additional
#include "Scene/Nodes/UIButton.h"

CUIButtonPass::CUIButtonPass(IRenderDevice& RenderDevice, std::weak_ptr<IScene> Scene)
	: BaseUIPass(RenderDevice, Scene)
{}

CUIButtonPass::~CUIButtonPass()
{}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIButtonPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::ConfigurePipeline(RenderTarget, Viewport);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Button.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Button.hlsl", "PS_main");

	// Create samplers
	std::shared_ptr<ISamplerState> sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);

	GetPipeline().SetSampler(0, sampler);

	// Material
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIButtonPass::Visit(const ISceneNodeUI * node)
{
	if (const CUIButtonNode* textNode = dynamic_cast<const CUIButtonNode*>(node))
	{
		return BaseUIPass::Visit(node);
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIButtonPass::Visit(const IModel * Model)
{
	return __super::Visit(Model);
}