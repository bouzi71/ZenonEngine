#include "stdafx.h"

// General
#include "ClearRenderTargetPass.h"

ClearRenderTargetPass::ClearRenderTargetPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> renderTarget, ClearFlags clearFlags, const glm::vec4& color, float depth, uint8_t stencil)
	: AbstractPass(RenderDevice)
	, m_RenderTarget(renderTarget)
	, m_ClearFlags(clearFlags)
	, m_ClearColor(color)
	, m_ClearDepth(depth)
	, m_ClearStencil(stencil)
{}

ClearRenderTargetPass::ClearRenderTargetPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<ITexture> texture, ClearFlags clearFlags, const glm::vec4& color, float depth, uint8_t stencil)
	: AbstractPass(RenderDevice)
	, m_Texture(texture)
	, m_ClearFlags(clearFlags)
	, m_ClearColor(color)
	, m_ClearDepth(depth)
	, m_ClearStencil(stencil)
{}

ClearRenderTargetPass::~ClearRenderTargetPass()
{}

void ClearRenderTargetPass::Render(RenderEventArgs& e)
{
	if (m_RenderTarget)
	{
		m_RenderTarget->Clear(m_ClearFlags, m_ClearColor, m_ClearDepth, m_ClearStencil);
	}
	if (m_Texture)
	{
		m_Texture->Clear(m_ClearFlags, m_ClearColor, m_ClearDepth, m_ClearStencil);
	}
}
