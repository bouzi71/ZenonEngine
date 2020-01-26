#include "stdafx.h"

#include "TextureDX11.h"
#include "StructuredBufferDX11.h"

#include "RenderTargetDX11.h"

RenderTargetDX11::RenderTargetDX11(IRenderDeviceDX11* RenderDeviceD3D11)
	: m_RenderDeviceD3D11(RenderDeviceD3D11)
	, m_Width(0)
	, m_Height(0)
	, m_bCheckValidity(false)
{
	m_Textures.resize((size_t)IRenderTarget::AttachmentPoint::NumAttachmentPoints + 1);
	m_StructuredBuffers.resize(8);
}

RenderTargetDX11::~RenderTargetDX11()
{

}

void RenderTargetDX11::AttachTexture(AttachmentPoint attachment, ITexture* texture)
{
	TextureDX11* textureDX11 = dynamic_cast<TextureDX11*>(texture);
	m_Textures[(uint8_t)attachment] = textureDX11;

	// Next time the render target is "bound", check that it is valid.
	m_bCheckValidity = true;
}

ITexture* RenderTargetDX11::GetTexture(AttachmentPoint attachment)
{
	return m_Textures[(uint8_t)attachment];
}


void RenderTargetDX11::Clear(AttachmentPoint attachment, ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	TextureDX11* texture = m_Textures[(uint8_t)attachment];
	if (texture)
	{
		texture->Clear(clearFlags, color, depth, stencil);
	}
}

void RenderTargetDX11::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	for (uint8_t i = 0; i < (uint8_t)AttachmentPoint::NumAttachmentPoints; ++i)
	{
		Clear((AttachmentPoint)i, clearFlags, color, depth, stencil);
	}
}

void RenderTargetDX11::GenerateMipMaps()
{
	for (auto texture : m_Textures)
	{
		if (texture)
		{
			texture->GenerateMipMaps();
		}
	}
}

void RenderTargetDX11::AttachStructuredBuffer(uint8_t slot, IStructuredBuffer* rwBuffer)
{
	StructuredBufferDX11* rwbufferDX11 = dynamic_cast<StructuredBufferDX11*>(rwBuffer);
	m_StructuredBuffers[slot] = rwbufferDX11;

	// Next time the render target is "bound", check that it is valid.
	m_bCheckValidity = true;
}

IStructuredBuffer* RenderTargetDX11::GetStructuredBuffer(uint8_t slot)
{
	if (slot < m_StructuredBuffers.size())
	{
		return m_StructuredBuffers[slot];
	}
	return nullptr;
}


void RenderTargetDX11::Resize(uint16_t width, uint16_t height)
{
	if (m_Width != width || m_Height != height)
	{
		m_Width = glm::max<uint16_t>(width, 1);
		m_Height = glm::max<uint16_t>(height, 1);

		// Resize the attached textures.
		for (const auto& texture : m_Textures)
		{
			if (texture)
			{
				texture->Resize(m_Width, m_Height);
			}
		}
	}
}

void RenderTargetDX11::Bind()
{
	if (m_bCheckValidity)
	{
		if (!IsValid())
		{
			Log::Error("Invalid render target.");
		}
		m_bCheckValidity = false;
	}

	ID3D11RenderTargetView* renderTargetViews[8];
	UINT numRTVs = 0;
	for (uint8_t i = 0; i < 8; i++)
		if (TextureDX11* texture = m_Textures[i])
			renderTargetViews[numRTVs++] = texture->GetRenderTargetView();

	ID3D11UnorderedAccessView* uavViews[8];
	UINT uavStartSlot = numRTVs;
	UINT numUAVs = 0;
	for (uint8_t i = 0; i < 8; i++)
		if (StructuredBufferDX11* rwbuffer = m_StructuredBuffers[i])
			uavViews[numUAVs++] = rwbuffer->GetUnorderedAccessView();

	ID3D11DepthStencilView* depthStencilView = nullptr;
	if (TextureDX11* depthTexture = m_Textures[(uint8_t)AttachmentPoint::Depth])
	{
		depthStencilView = depthTexture->GetDepthStencilView();
	}
	else if (TextureDX11* depthStencilTexture = m_Textures[(uint8_t)AttachmentPoint::DepthStencil])
	{
		depthStencilView = depthStencilTexture->GetDepthStencilView();
	}

	m_RenderDeviceD3D11->GetDeviceContextD3D11()->OMSetRenderTargetsAndUnorderedAccessViews(numRTVs, renderTargetViews, depthStencilView, uavStartSlot, numUAVs, uavViews, nullptr);
}

void RenderTargetDX11::UnBind()
{
	m_RenderDeviceD3D11->GetDeviceContextD3D11()->OMSetRenderTargetsAndUnorderedAccessViews(0, nullptr, nullptr, 0, 0, nullptr, nullptr);
}

bool RenderTargetDX11::IsValid() const
{
	UINT numRTV = 0;
	int width = -1;
	int height = -1;

	for (auto texture : m_Textures)
	{
		if (texture)
		{
			if (texture->GetRenderTargetView()) 
				++numRTV;

			if (width == -1 || height == -1)
			{
				width = texture->GetWidth();
				height = texture->GetHeight();
			}
			else
			{
				if (texture->GetWidth() != width || texture->GetHeight() != height)
				{
					return false;
				}
			}
		}
	}

	UINT numUAV = 0;
	for (auto rwBuffer : m_StructuredBuffers)
	{
		if (rwBuffer)
		{
			++numUAV;
		}
	}

	if (numRTV + numUAV > 8)
	{
		return false;
	}

	return true;
}


