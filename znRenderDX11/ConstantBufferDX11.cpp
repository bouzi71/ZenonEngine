#include <stdafx.h>

#include "ConstantBufferDX11.h"

ConstantBufferDX11::ConstantBufferDX11(ID3D11Device2* pDevice, size_t size)
	: m_pDevice(pDevice)
	, m_BufferSize(size)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = (UINT)m_BufferSize;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	if (FAILED(m_pDevice->CreateBuffer(&bufferDesc, nullptr, &m_pBuffer)))
	{
		fail2("Failed to create constant buffer for shader.");
		return;
	}

	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

ConstantBufferDX11::~ConstantBufferDX11()
{}

void ConstantBufferDX11::Set(const void* data, size_t size)
{
	_ASSERT(size == m_BufferSize);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(m_pDeviceContext->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		fail2("Failed to map constant buffer.");
		return;
	}

	memcpy(mappedResource.pData, data, m_BufferSize);

	m_pDeviceContext->Unmap(m_pBuffer, 0);
}

void ConstantBufferDX11::Copy(std::shared_ptr<IConstantBuffer> other)
{
	std::shared_ptr<ConstantBufferDX11> srcBuffer = std::dynamic_pointer_cast<ConstantBufferDX11>(other);

	if (srcBuffer && srcBuffer.get() != this &&
		m_BufferSize == srcBuffer->m_BufferSize)
	{
		m_pDeviceContext->CopyResource(m_pBuffer, srcBuffer->m_pBuffer);
	}
	else
	{
		Log::Error("Source buffer is not compatible with this buffer.");
	}
}

void ConstantBufferDX11::Copy(std::shared_ptr<IBuffer> other)
{
	Copy(std::dynamic_pointer_cast<IConstantBuffer>(other));
}

bool ConstantBufferDX11::Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	bool result = true;

	ID3D11Buffer* pBuffers[] = { m_pBuffer };

	switch (shader->GetType())
	{
	case IShader::ShaderType::VertexShader:
		m_pDeviceContext->VSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::TessellationControlShader:
		m_pDeviceContext->HSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::TessellationEvaluationShader:
		m_pDeviceContext->DSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::GeometryShader:
		m_pDeviceContext->GSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::PixelShader:
		m_pDeviceContext->PSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::ComputeShader:
		m_pDeviceContext->CSSetConstantBuffers(id, 1, pBuffers);
		break;
	default:
		result = false;
		break;
	}

	return result;
}

void ConstantBufferDX11::UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const
{
	ID3D11Buffer* pBuffers[] = { nullptr };

	switch (shader->GetType())
	{
	case IShader::ShaderType::VertexShader:
		m_pDeviceContext->VSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::TessellationControlShader:
		m_pDeviceContext->HSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::TessellationEvaluationShader:
		m_pDeviceContext->DSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::GeometryShader:
		m_pDeviceContext->GSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::PixelShader:
		m_pDeviceContext->PSSetConstantBuffers(id, 1, pBuffers);
		break;
	case IShader::ShaderType::ComputeShader:
		m_pDeviceContext->CSSetConstantBuffers(id, 1, pBuffers);
		break;
	default:
		break;
	}
}