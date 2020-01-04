#include "stdafx.h"

// General
#include "TextureDX11.h"

// Additional
#include "TextureDX11Translate.h"

TextureDX11::TextureDX11(std::weak_ptr<IRenderDevice> RenderDevice)
	: m_RenderDevice(RenderDevice)
	, m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_NumSlices(0)
	, m_TextureResourceFormatSupport(0)
	, m_DepthStencilViewFormatSupport(0)
	, m_ShaderResourceViewFormatSupport(0)
	, m_RenderTargetViewFormatSupport(0)
	, m_CPUAccess(CPUAccess::None)
	, m_bDynamic(false)
	, m_bUAV(false)
	, m_TextureResourceFormat(DXGI_FORMAT_UNKNOWN)
	, m_DepthStencilViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_ShaderResourceViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_RenderTargetViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_bGenerateMipmaps(false)
	, m_BPP(0)
	, m_Pitch(0)
	, m_bIsTransparent(false)
	, m_bIsDirty(false)
{
	m_pDevice = std::dynamic_pointer_cast<IRenderDeviceDX11>(m_RenderDevice.lock())->GetDevice();
	m_pDeviceContext = std::dynamic_pointer_cast<IRenderDeviceDX11>(m_RenderDevice.lock())->GetDeviceContext();
}

// 2D Texture
TextureDX11::TextureDX11(std::weak_ptr<IRenderDevice> RenderDevice, uint16_t width, uint16_t height, uint16_t slices, const ITexture::TextureFormat& format, CPUAccess cpuAccess, bool bUAV)
	: m_RenderDevice(RenderDevice)
	, m_pTexture2D(nullptr)
	, m_pShaderResourceView(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_BPP(0)
	, m_TextureFormat(format)
	, m_CPUAccess(cpuAccess)
	, m_bGenerateMipmaps(false)
	, m_bIsTransparent(true)
	, m_bIsDirty(false)
{
	m_pDevice = std::dynamic_pointer_cast<IRenderDeviceDX11>(m_RenderDevice.lock())->GetDevice();
	m_pDeviceContext = std::dynamic_pointer_cast<IRenderDeviceDX11>(m_RenderDevice.lock())->GetDeviceContext();

	m_NumSlices = glm::max<uint16_t>(slices, 1);

	m_TextureDimension = ITexture::Dimension::Texture2D;
	if (m_NumSlices > 1)
	{
		m_TextureDimension = ITexture::Dimension::Texture2DArray;
	}

	// Translate to DXGI format.
	DXGI_FORMAT dxgiFormat = DX11TranslateFormat(format);
	m_SampleDesc = GetSupportedSampleCount(dxgiFormat, format.NumSamples);

	// Translate back to original format (for best match format).
	m_TextureFormat = DX11TranslateFormat(dxgiFormat, format.NumSamples);
	// Convert Depth/Stencil formats to typeless texture resource formats.
	m_TextureResourceFormat = DX11GetTextureFormat(dxgiFormat);
	// Convert typeless formats to Depth/Stencil view formats.
	m_DepthStencilViewFormat = DX11GetDSVFormat(dxgiFormat);
	// Convert depth/stencil and typeless formats to Shader Resource View formats.
	m_ShaderResourceViewFormat = DX11GetSRVFormat(dxgiFormat);
	// Convert typeless formats to Render Target View formats.
	m_RenderTargetViewFormat = DX11GetRTVFormat(dxgiFormat);
	// Convert typeless format to Unordered Access View formats.
	m_UnorderedAccessViewFormat = DX11GetUAVFormat(dxgiFormat);

	m_BPP = DX11GetBPP(m_TextureResourceFormat);

	// Query for texture format support.
	if (FAILED(m_pDevice->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
	{
		Log::Error("Failed to query texture resource format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_DepthStencilViewFormat, &m_DepthStencilViewFormatSupport)))
	{
		Log::Error("Failed to query depth/stencil format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_ShaderResourceViewFormat, &m_ShaderResourceViewFormatSupport)))
	{
		Log::Error("Failed to query shader resource format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_RenderTargetViewFormat, &m_RenderTargetViewFormatSupport)))
	{
		Log::Error("Failed to query render target format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_UnorderedAccessViewFormat, &m_UnorderedAccessViewFormatSupport)))
	{
		Log::Error("Failed to query render target format support.");
	}
	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
	{
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");
	}
	// Can the texture be dynamically modified on the CPU?
	m_bDynamic = (int)m_CPUAccess != 0 && (m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_CPU_LOCKABLE) != 0;
	// Can mipmaps be automatically generated for this texture format?
	m_bGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;
	// Are UAVs supported?
	m_bUAV = bUAV && (m_UnorderedAccessViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_LOAD) != 0;

	Resize(width, height);
}

// CUBE Texture
TextureDX11::TextureDX11(std::weak_ptr<IRenderDevice> RenderDevice, uint16_t size, uint16_t count, const TextureFormat& format, CPUAccess cpuAccess, bool bUAV)
	: m_RenderDevice(RenderDevice)
{
	m_pDevice = std::dynamic_pointer_cast<IRenderDeviceDX11>(m_RenderDevice.lock())->GetDevice();
	m_pDeviceContext = std::dynamic_pointer_cast<IRenderDeviceDX11>(m_RenderDevice.lock())->GetDeviceContext();

	m_TextureDimension = ITexture::Dimension::TextureCube;

	m_TextureWidth = m_TextureHeight = size;

	// Translate to DXGI format.
	DXGI_FORMAT dxgiFormat = DX11TranslateFormat(format);
	m_SampleDesc = GetSupportedSampleCount(dxgiFormat, format.NumSamples);

	// Translate back to original format (for best match format)
	m_TextureFormat = DX11TranslateFormat(dxgiFormat, format.NumSamples);

	// Convert Depth/Stencil formats to typeless texture resource formats
	m_TextureResourceFormat = DX11GetTextureFormat(dxgiFormat);
	// Convert typeless formats to Depth/Stencil view formats
	m_DepthStencilViewFormat = DX11GetDSVFormat(dxgiFormat);
	// Convert depth/stencil and typeless formats to Shader Resource View formats
	m_ShaderResourceViewFormat = DX11GetSRVFormat(dxgiFormat);
	// Convert typeless formats to Render Target View formats
	m_RenderTargetViewFormat = DX11GetRTVFormat(dxgiFormat);
	// Convert typeless format to Unordered Access View formats.
	m_UnorderedAccessViewFormat = DX11GetUAVFormat(dxgiFormat);

	// Query for texture format support.
	if (FAILED(m_pDevice->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
	{
		Log::Error("Failed to query texture resource format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_DepthStencilViewFormat, &m_DepthStencilViewFormatSupport)))
	{
		Log::Error("Failed to query depth/stencil format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_ShaderResourceViewFormat, &m_ShaderResourceViewFormatSupport)))
	{
		Log::Error("Failed to query shader resource format support.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_RenderTargetViewFormat, &m_RenderTargetViewFormatSupport)))
	{
		Log::Error("Failed to query render target format support.");
	}
	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURECUBE) == 0)
	{
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for cubemap textures.");
	}
	if (FAILED(m_pDevice->CheckFormatSupport(m_UnorderedAccessViewFormat, &m_UnorderedAccessViewFormatSupport)))
	{
		Log::Error("Failed to query render target format support.");
	}

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURECUBE) == 0)
	{
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for cube textures.");
	}

	// Can the texture be dynamically modified on the CPU?
	m_bDynamic = ((int)m_CPUAccess & (int)CPUAccess::Write) != 0 && (m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_CPU_LOCKABLE) != 0;
	// Can mipmaps be automatically generated for this texture format?
	m_bGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0; // && ( m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN ) != 0;
	// Are UAVs supported?
	m_bUAV = bUAV && (m_UnorderedAccessViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_LOAD) != 0;

}

TextureDX11::~TextureDX11()
{}



void TextureDX11::GenerateMipMaps()
{
	if (m_bGenerateMipmaps && m_pShaderResourceView)
	{
		m_pDeviceContext->GenerateMips(m_pShaderResourceView);
	}
}

std::shared_ptr<ITexture> TextureDX11::GetFace(CubeFace face) const
{
	return std::static_pointer_cast<ITexture>(std::const_pointer_cast<TextureDX11>(shared_from_this()));
}

std::shared_ptr<ITexture> TextureDX11::GetSlice(uint32 slice) const
{
	return std::static_pointer_cast<ITexture>(std::const_pointer_cast<TextureDX11>(shared_from_this()));
}

uint16_t TextureDX11::GetWidth() const
{
	return m_TextureWidth;
}

uint16_t TextureDX11::GetHeight() const
{
	return m_TextureHeight;
}

glm::ivec2 TextureDX11::GetSize() const
{
	return glm::ivec2(m_TextureWidth, m_TextureHeight);
}

uint16_t TextureDX11::GetDepth() const
{
	return m_NumSlices;
}

uint8_t TextureDX11::GetBPP() const
{
	return m_BPP;
}

bool TextureDX11::IsTransparent() const
{
	return m_bIsTransparent;
}

void TextureDX11::Resize2D(uint16_t width, uint16_t height)
{
	if (m_TextureWidth != width || m_TextureHeight != height)
	{
		// Release resource before resizing
		m_pTexture2D.Release();
		m_pRenderTargetView.Release();
		m_pDepthStencilView.Release();
		m_pShaderResourceView.Release();
		m_pUnorderedAccessView.Release();

		m_TextureWidth = glm::max<uint16_t>(width, 1);
		m_TextureHeight = glm::max<uint16_t>(height, 1);

		// Create texture with the dimensions specified.
		D3D11_TEXTURE2D_DESC textureDesc = { 0 };
		textureDesc.ArraySize = m_NumSlices;
		textureDesc.Format = m_TextureResourceFormat;
		textureDesc.SampleDesc = m_SampleDesc;
		textureDesc.Width = m_TextureWidth;
		textureDesc.Height = m_TextureHeight;
		textureDesc.MipLevels = 1;

		if (((int)m_CPUAccess & (int)CPUAccess::Read) != 0)
		{
			textureDesc.Usage = D3D11_USAGE_STAGING;
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		}
		else if (((int)m_CPUAccess & (int)CPUAccess::Write) != 0)
		{
			textureDesc.Usage = D3D11_USAGE_DYNAMIC;
			textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.CPUAccessFlags = 0;
		}

		if (!m_bUAV && !m_bDynamic && (m_DepthStencilViewFormatSupport & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL) != 0)
		{
			textureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		}
		if (!m_bDynamic && (m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_RENDER_TARGET) != 0)
		{
			textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		if (((int)m_CPUAccess & (int)CPUAccess::Read) == 0)
		{
			textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
		}
		if (m_bUAV && !m_bDynamic)
		{
			textureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}

		textureDesc.MiscFlags = m_bGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pTexture2D)))
		{
			Log::Error("Failed to create texture.");
			return;
		}

		if ((textureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL) != 0)
		{
			// Create the depth/stencil view for the texture.
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			depthStencilViewDesc.Format = m_DepthStencilViewFormat;
			depthStencilViewDesc.Flags = 0;

			if (m_NumSlices > 1)
			{
				if (m_SampleDesc.Count > 1)
				{
					depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
					depthStencilViewDesc.Texture2DMSArray.FirstArraySlice = 0;
					depthStencilViewDesc.Texture2DMSArray.ArraySize = m_NumSlices;
				}
				else
				{
					depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
					depthStencilViewDesc.Texture2DArray.MipSlice = 0;
					depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
					depthStencilViewDesc.Texture2DArray.ArraySize = m_NumSlices;
				}
			}
			else
			{
				if (m_SampleDesc.Count > 1)
				{
					depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				}
				else
				{
					depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					depthStencilViewDesc.Texture2D.MipSlice = 0;
				}
			}

			if (FAILED(m_pDevice->CreateDepthStencilView(m_pTexture2D, &depthStencilViewDesc, &m_pDepthStencilView)))
			{
				Log::Error("Failed to create depth/stencil view.");
			}
		}

		if ((textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0)
		{
			// Create a Shader resource view for the texture.
			D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
			resourceViewDesc.Format = m_ShaderResourceViewFormat;

			if (m_NumSlices > 1)
			{
				if (m_SampleDesc.Count > 1)
				{
					resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
					resourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;
					resourceViewDesc.Texture2DMSArray.ArraySize = m_NumSlices;
				}
				else
				{
					resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
					resourceViewDesc.Texture2DArray.FirstArraySlice = 0;
					resourceViewDesc.Texture2DArray.ArraySize = m_NumSlices;
					resourceViewDesc.Texture2DArray.MipLevels = m_bGenerateMipmaps ? -1 : 1;
					resourceViewDesc.Texture2DArray.MostDetailedMip = 0;
				}
			}
			else
			{
				if (m_SampleDesc.Count > 1)
				{
					resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				}
				else
				{
					resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
					resourceViewDesc.Texture2D.MipLevels = m_bGenerateMipmaps ? -1 : 1;
					resourceViewDesc.Texture2D.MostDetailedMip = 0;
				}
			}

			if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, &resourceViewDesc, &m_pShaderResourceView)))
			{
				Log::Error("Failed to create texture resource view.");
			}
			else if (m_bGenerateMipmaps)
			{
				m_pDeviceContext->GenerateMips(m_pShaderResourceView);
			}
		}

		if ((textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET) != 0)
		{
			// Create the render target view for the texture.
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			renderTargetViewDesc.Format = m_RenderTargetViewFormat;

			if (m_NumSlices > 1)
			{
				if (m_SampleDesc.Count > 1)
				{
					renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
					renderTargetViewDesc.Texture2DArray.ArraySize = m_NumSlices;

				}
				else
				{
					renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					renderTargetViewDesc.Texture2DArray.MipSlice = 0;
					renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
					renderTargetViewDesc.Texture2DArray.ArraySize = m_NumSlices;
				}
			}
			else
			{
				if (m_SampleDesc.Count > 1)
				{
					renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
				}
				else
				{
					renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					renderTargetViewDesc.Texture2D.MipSlice = 0;
				}
			}

			if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, &renderTargetViewDesc, &m_pRenderTargetView)))
			{
				Log::Error("Failed to create render target view.");
			}
		}

		if ((textureDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0)
		{
			// UAVs cannot be multi sampled.
			assert(m_SampleDesc.Count == 1);

			// Create a Shader resource view for the texture.
			D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc;
			unorderedAccessViewDesc.Format = m_UnorderedAccessViewFormat;

			if (m_NumSlices > 1)
			{
				unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				unorderedAccessViewDesc.Texture2DArray.MipSlice = 0;
				unorderedAccessViewDesc.Texture2DArray.FirstArraySlice = 0;
				unorderedAccessViewDesc.Texture2DArray.ArraySize = m_NumSlices;
			}
			else
			{
				unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				unorderedAccessViewDesc.Texture2D.MipSlice = 0;
			}

			if (FAILED(m_pDevice->CreateUnorderedAccessView(m_pTexture2D, &unorderedAccessViewDesc, &m_pUnorderedAccessView)))
			{
				Log::Error("Failed to create unordered access view.");
			}
		}

		assert(m_BPP > 0 && m_BPP % 8 == 0);
		m_Buffer.resize(width * height * (m_BPP / 8));
	}
}

void TextureDX11::ResizeCube(uint16_t size)
{
	// TODO
}

void TextureDX11::Resize(uint16_t width, uint16_t height, uint16_t depth)
{
	switch (m_TextureDimension)
	{
	case Dimension::Texture2D:
	case Dimension::Texture2DArray:
		Resize2D(width, height);
		break;
	case ITexture::Dimension::TextureCube:
		ResizeCube(width);
		break;
	default:
		Log::Error("Unknown texture dimension.");
		break;
	}

	return;
}

void TextureDX11::Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size)
{
	assert(m_BPP > 0 && m_BPP % 8 == 0);
	assert(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8_t bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);

	for (uint32 i = 0; i < size; ++i)
	{
		m_Buffer[index + i] = *(pixel + i);
	}

	m_bIsDirty = true;
}

void TextureDX11::FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size)
{
	assert(m_BPP > 0 && m_BPP % 8 == 0);
	assert(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8_t bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);
	pixel = &m_Buffer[index];
}

void TextureDX11::Copy(std::shared_ptr<ITexture> other)
{
	std::shared_ptr<TextureDX11> srcTexture = std::dynamic_pointer_cast<TextureDX11>(other);

	if (srcTexture && srcTexture.get() != this)
	{
		if (m_TextureDimension == srcTexture->m_TextureDimension &&
			m_TextureWidth == srcTexture->m_TextureWidth &&
			m_TextureHeight == srcTexture->m_TextureHeight)
		{
			switch (m_TextureDimension)
			{
			case ITexture::Dimension::Texture2D:
			case ITexture::Dimension::Texture2DArray:
				m_pDeviceContext->CopyResource(m_pTexture2D, srcTexture->m_pTexture2D);
				break;
			case ITexture::Dimension::TextureCube:
				m_pDeviceContext->CopyResource(m_pTexture3D, srcTexture->m_pTexture3D);
				break;
			}
		}
		else
		{
			Log::Error("Incompatible source texture.");
		}
	}

	if (((int)m_CPUAccess & (int)CPUAccess::Read) != 0 && m_pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Copy the texture data from the texture resource
		if (FAILED(m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &mappedResource)))
		{
			Log::Error("Failed to map texture resource for reading.");
		}

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_pDeviceContext->Unmap(m_pTexture2D, 0);
	}
}

void TextureDX11::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	if (m_pRenderTargetView && ((int)clearFlags & (int)ClearFlags::Color) != 0)
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, glm::value_ptr(color));
	}

	{
		UINT flags = 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Depth) != 0 ? D3D11_CLEAR_DEPTH : 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Stencil) != 0 ? D3D11_CLEAR_STENCIL : 0;
		if (m_pDepthStencilView && flags > 0)
		{
			m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, flags, depth, stencil);
		}
	}
}

void TextureDX11::Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const 
{
	Bind(ID, shader->GetType(), parameterType);
}

void TextureDX11::Bind(uint32_t ID, SShaderType _shaderType, IShaderParameter::Type parameterType) const
{
	if (m_bIsDirty)
	{
		if (m_bDynamic && m_pTexture2D)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			// Copy the texture data to the texture resource
			HRESULT hr = m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				Log::Error("Failed to map texture resource for writing.");
			}

			memcpy_s(mappedResource.pData, m_Buffer.size(), m_Buffer.data(), m_Buffer.size());

			m_pDeviceContext->Unmap(m_pTexture2D, 0);

			if (m_bGenerateMipmaps)
			{
				m_pDeviceContext->GenerateMips(m_pShaderResourceView);
			}
		}
		m_bIsDirty = false;
	}

	ID3D11ShaderResourceView* srv[] = { m_pShaderResourceView };
	ID3D11UnorderedAccessView* uav[] = { m_pUnorderedAccessView };

	if (parameterType == IShaderParameter::Type::Texture && m_pShaderResourceView)
	{
		switch (_shaderType)
		{
		case SShaderType::VertexShader:
			m_pDeviceContext->VSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::TessellationControlShader:
			m_pDeviceContext->HSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::TessellationEvaluationShader:
			m_pDeviceContext->DSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::GeometryShader:
			m_pDeviceContext->GSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::PixelShader:
			m_pDeviceContext->PSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::ComputeShader:
			m_pDeviceContext->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == IShaderParameter::Type::RWTexture && m_pUnorderedAccessView)
	{
		switch (_shaderType)
		{
		case SShaderType::ComputeShader:
			m_pDeviceContext->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}
}


void TextureDX11::UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	UnBind(ID, shader->GetType(), parameterType);
}

void TextureDX11::UnBind(uint32_t ID, SShaderType _shaderType, IShaderParameter::Type parameterType) const
{
	ID3D11ShaderResourceView* srv[] = { nullptr };
	ID3D11UnorderedAccessView* uav[] = { nullptr };

	if (parameterType == IShaderParameter::Type::Texture)
	{
		switch (_shaderType)
		{
		case SShaderType::VertexShader:
			m_pDeviceContext->VSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::TessellationControlShader:
			m_pDeviceContext->HSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::TessellationEvaluationShader:
			m_pDeviceContext->DSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::GeometryShader:
			m_pDeviceContext->GSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::PixelShader:
			m_pDeviceContext->PSSetShaderResources(ID, 1, srv);
			break;
		case SShaderType::ComputeShader:
			m_pDeviceContext->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == IShaderParameter::Type::RWTexture)
	{
		switch (_shaderType)
		{
		case SShaderType::ComputeShader:
			m_pDeviceContext->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}
}

DXGI_SAMPLE_DESC TextureDX11::GetSupportedSampleCount(DXGI_FORMAT format, uint8_t numSamples)
{
	DXGI_SAMPLE_DESC sampleDesc;

	UINT sampleCount = 1;
	UINT qualityLevels = 0;

	while (sampleCount <= numSamples && SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(format, sampleCount, &qualityLevels)) && qualityLevels > 0)
	{
		// That works...
		sampleDesc.Count = sampleCount;
		sampleDesc.Quality = qualityLevels - 1;

		// But can we do better?
		sampleCount = sampleCount * 2;
	}

	return sampleDesc;
}

const std::vector<uint8>& TextureDX11::GetBuffer()
{
	/*if (((int)m_CPUAccess & (int)CPUAccess::Read) != 0 && m_pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Copy the texture data from the texture resource
		if (FAILED(m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &mappedResource)))
		{
			Log::Error("Failed to map texture resource for reading.");
		}

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_pDeviceContext->Unmap(m_pTexture2D, 0);
	}*/

	return m_Buffer;
}

ID3D11Resource* TextureDX11::GetTextureResource() const
{
	ID3D11Resource* resource = nullptr;
	switch (m_TextureDimension)
	{
	case ITexture::Dimension::Texture2D:
	case ITexture::Dimension::Texture2DArray:
		resource = m_pTexture2D;
		break;
	case ITexture::Dimension::TextureCube:
		resource = m_pTexture3D;
		break;
	}

	return resource;
}

ID3D11ShaderResourceView* TextureDX11::GetShaderResourceView() const
{
	return m_pShaderResourceView;
}

ID3D11DepthStencilView* TextureDX11::GetDepthStencilView() const
{
	return m_pDepthStencilView;
}

ID3D11RenderTargetView* TextureDX11::GetRenderTargetView() const
{
	return m_pRenderTargetView;
}

ID3D11UnorderedAccessView* TextureDX11::GetUnorderedAccessView() const
{
	return m_pUnorderedAccessView;
}

