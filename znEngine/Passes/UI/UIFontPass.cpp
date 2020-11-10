#include "stdafx.h"

// General
#include "UIFontPass.h"

// Additional
#include "Scene/Nodes/UIText.h"


CUIFontPass::CUIFontPass(IRenderDevice& RenderDevice, IScene& Scene)
	: BaseUIPass(Scene)
{
	m_FontBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SFontPerCharacterData());
}

CUIFontPass::~CUIFontPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIFontPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	BaseUIPass::ConfigurePipeline(RenderTarget, Viewport);

	// CreateShaders
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Font.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Font.hlsl", "PS_main");
	}
	else if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_OpenGL)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "IDB_SHADER_OGL__UI_FONT_VS", "");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "IDB_SHADER_OGL__UI_FONT_PS", "");
	}

	vertexShader->LoadInputLayoutFromReflector();

	// Create samplers
	std::shared_ptr<ISamplerState> linearClampSampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	linearClampSampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	linearClampSampler->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	GetPipeline().SetSampler(0, linearClampSampler);

	// Material
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CUIFontPass::Visit(const IUIControl * node)
{
	const CUITextNode* textNode = dynamic_cast<const CUITextNode*>(node);
	if (textNode == nullptr)
		return EVisitResult::AllowVisitChilds;

	BaseUIPass::Visit(node);

	const auto& textToRender = textNode->GetText();
	const auto& color = textNode->GetColor();
	const auto& font = textNode->GetFont();

	const auto& fontGeometry = font->GetGeometry();
	const auto& fontGeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(fontGeometry);

	const auto& shaders = GetPipeline().GetShaders();
	auto vertexShader = shaders.at(EShaderType::VertexShader).get();
	auto pixelShader = shaders.at(EShaderType::PixelShader).get();

	GetPipeline().SetTexture(0, font->GetTexture());

	fontGeometryInternal->Render_BindAllBuffers(vertexShader);
	{
		auto currentCharOffset = textNode->GetOffset();

		for (size_t i = 0; i < textToRender.length(); i++)
		{
			const uint8 ch = static_cast<uint8>(textToRender.c_str()[i]);
			if (ch == '\n')
			{
				currentCharOffset.x = textNode->GetOffset().x;
				currentCharOffset.y += static_cast<float>(font->GetHeight()) + 0.01f;
				continue;
			}

			{
				SFontPerCharacterData fontProperties;
				fontProperties.Color = color;
				fontProperties.Offset = currentCharOffset;
			
				BindPerCharacterData(fontProperties);
			}

			{
				SGeometryDrawArgs GeometryDrawArgs;
				GeometryDrawArgs.VertexStartLocation = (ch) * 6;
				GeometryDrawArgs.VertexCnt = 6;
				fontGeometryInternal->Render_Draw(GeometryDrawArgs);
			}

			currentCharOffset.x += static_cast<float>(font->GetWidth(ch)) + 0.01f;
		}
	}
	fontGeometryInternal->Render_UnbindAllBuffers(vertexShader);

	return EVisitResult::AllowVisitChilds;
}



//
// Private
//
void CUIFontPass::BindPerCharacterData(const SFontPerCharacterData& PerCharacterData)
{
	m_FontBuffer->Set(PerCharacterData);

	for (const auto& shaderIt : GetPipeline().GetShaders())
	{
		auto& perFrameParam = shaderIt.second->GetShaderParameterByName("PerCharacterData");
		if (perFrameParam.IsValid())
		{
			perFrameParam.SetConstantBuffer(m_FontBuffer);
			perFrameParam.Bind();
		}
	}
}
