#include "stdafx.h"

// Include render
#include "BufferDX11.h"
#include "ShaderDX11.h"
#include "PipelineStateDX11.h"

// General
#include "GeometryDX11.h"

GeometryDX11::GeometryDX11(IRenderDeviceDX11* RenderDeviceD3D11)
	: m_RenderDeviceD3D11(RenderDeviceD3D11)
	, m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{}

GeometryDX11::~GeometryDX11()
{}

void GeometryDX11::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	switch (_topology)
	{
	case PrimitiveTopology::PointList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case PrimitiveTopology::LineList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ; // TODO REMOVE ME!
		break;
	case PrimitiveTopology::LineStrip:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		break;
	case PrimitiveTopology::TriangleList:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case PrimitiveTopology::TriangleStrip:
		m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	}
}

bool GeometryDX11::Render(const RenderEventArgs* renderArgs, const IConstantBuffer* PerObject, const std::unordered_map<EShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams) const
{
	UINT indexStartLocation = GeometryPartParams.IndexStartLocation;
	UINT indexCnt = GeometryPartParams.IndexCnt;
    if (indexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
        indexCnt = m_pIndexBuffer->GetElementCount();

	UINT vertexStartLocation = GeometryPartParams.VertexStartLocation;
	UINT vertexCnt = GeometryPartParams.VertexCnt;
	if (vertexCnt == UINT_MAX)
	{
		if (m_VertexBuffer != nullptr)
		{
			vertexCnt = m_VertexBuffer->GetElementCount();
		}
		else if (!m_VertexBuffers.empty())
		{
			vertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
		}
		else
		{
			_ASSERT(false);
		}
	}

	const IShader* vertexShader = ShadersMap.at(EShaderType::VertexShader).get();
	_ASSERT(vertexShader != nullptr);

	const auto& geomShaderIt = ShadersMap.find(EShaderType::GeometryShader);
	const IShader* geomShader = nullptr;
	if (geomShaderIt != ShadersMap.end())
	{
		geomShader = geomShaderIt->second.get();
	}

	IShaderParameter* perObjectParameter = vertexShader->GetShaderParameterByName("PerObject").get();
	if (perObjectParameter->IsValid() && PerObject != nullptr)
	{
		perObjectParameter->SetConstantBuffer(PerObject);
		perObjectParameter->Bind();
	}

	if (geomShader)
	{
		const std::shared_ptr<IShaderParameter>& perObjectParameterGS = geomShader->GetShaderParameterByName("PerObject");
		if (perObjectParameterGS->IsValid() && PerObject != nullptr)
		{
			perObjectParameterGS->SetConstantBuffer(PerObject);
			perObjectParameterGS->Bind();
		}
	}

	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Bind(0, vertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const BufferMap::value_type& buffer : m_VertexBuffers)
		{
			const BufferBinding& binding = buffer.first;
			if (vertexShader->GetInputLayout()->HasSemantic(binding))
			{
				UINT slotID = vertexShader->GetInputLayout()->GetSemanticSlot(binding);
				buffer.second->Bind(slotID, vertexShader, IShaderParameter::Type::Buffer);
			}
		}
	}

	m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Bind(0, vertexShader, IShaderParameter::Type::Buffer);
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->DrawIndexed(indexCnt, indexStartLocation, vertexStartLocation);
		m_pIndexBuffer->UnBind(0, vertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->Draw(vertexCnt, vertexStartLocation);
	}

	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->UnBind(0, vertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const BufferMap::value_type& buffer : m_VertexBuffers)
		{
			const BufferBinding& binding = buffer.first;
			if (vertexShader->GetInputLayout()->HasSemantic(binding))
			{
				UINT slotID = vertexShader->GetInputLayout()->GetSemanticSlot(binding);
				buffer.second->UnBind(slotID, vertexShader, IShaderParameter::Type::Buffer);
			}
		}
	}

	perObjectParameter = vertexShader->GetShaderParameterByName("PerObject").get();
	if (perObjectParameter->IsValid())
	{
		perObjectParameter->Unbind();
	}

	return true;
}

bool GeometryDX11::RenderInstanced(const RenderEventArgs * renderArgs, const IStructuredBuffer * InstancesBuffer, const std::unordered_map<EShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, SGeometryPartParams GeometryPartParams) const
{
	UINT indexStartLocation = GeometryPartParams.IndexStartLocation;
	UINT indexCnt = GeometryPartParams.IndexCnt;
	if (indexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
		indexCnt = m_pIndexBuffer->GetElementCount();

	UINT vertexStartLocation = GeometryPartParams.VertexStartLocation;
	UINT vertexCnt = GeometryPartParams.VertexCnt;
	if (vertexCnt == UINT_MAX)
	{
		if (m_VertexBuffer != nullptr)
		{
			vertexCnt = m_VertexBuffer->GetElementCount();
		}
		else if (!m_VertexBuffers.empty())
		{
			vertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
		}
		else
		{
			_ASSERT(false);
		}
	}

	const IShader* vertexShader = ShadersMap.at(EShaderType::VertexShader).get();
	_ASSERT(vertexShader != nullptr);

	const auto& geomShaderIt = ShadersMap.find(EShaderType::GeometryShader);
	const IShader* geomShader = nullptr;
	if (geomShaderIt != ShadersMap.end())
	{
		geomShader = geomShaderIt->second.get();
	}

	std::shared_ptr<IShaderParameter> instancesBufferParameter = vertexShader->GetShaderParameterByName("Instances");
	if (instancesBufferParameter->IsValid() && InstancesBuffer != nullptr)
	{
		instancesBufferParameter->SetStructuredBuffer(InstancesBuffer);
		instancesBufferParameter->Bind();
	}

	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->Bind(0, vertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const BufferMap::value_type& buffer : m_VertexBuffers)
		{
			const BufferBinding& binding = buffer.first;
			if (vertexShader->GetInputLayout()->HasSemantic(binding))
			{
				UINT slotID = vertexShader->GetInputLayout()->GetSemanticSlot(binding);
				buffer.second->Bind(slotID, vertexShader, IShaderParameter::Type::Buffer);
			}
		}
	}

	m_RenderDeviceD3D11->GetDeviceContextD3D11()->IASetPrimitiveTopology(m_PrimitiveTopology);

	if (m_pIndexBuffer != NULL)
	{
		m_pIndexBuffer->Bind(0, vertexShader, IShaderParameter::Type::Buffer);
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->DrawIndexedInstanced(indexCnt, InstancesBuffer->GetElementCount(), indexStartLocation, vertexStartLocation, 2);
		m_pIndexBuffer->UnBind(0, vertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->DrawInstanced(vertexCnt, InstancesBuffer->GetElementCount(), vertexStartLocation, 0);
	}

	if (m_VertexBuffer != nullptr)
	{
		m_VertexBuffer->UnBind(0, vertexShader, IShaderParameter::Type::Buffer);
	}
	else
	{
		for (const BufferMap::value_type& buffer : m_VertexBuffers)
		{
			const BufferBinding& binding = buffer.first;
			if (vertexShader->GetInputLayout()->HasSemantic(binding))
			{
				UINT slotID = vertexShader->GetInputLayout()->GetSemanticSlot(binding);
				buffer.second->UnBind(slotID, vertexShader, IShaderParameter::Type::Buffer);
			}
		}
	}

	instancesBufferParameter = vertexShader->GetShaderParameterByName("Instances");
	if (instancesBufferParameter->IsValid())
	{
		instancesBufferParameter->Unbind();
	}

	return true;
}
