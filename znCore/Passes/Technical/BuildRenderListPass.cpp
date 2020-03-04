#include "stdafx.h"

// General
#include "BuildRenderListPass.h"

#if 0

// Additional
#include <omp.h>

int threadCnt = omp_get_max_threads();
Log::Print("BuildRenderListPass: Threads cnt = '%d'", threadCnt);
m_PerObjectData.resize(threadCnt);
m_PerObjectConstantBuffer.resize(threadCnt);

for (int i = 0; i < threadCnt; i++)
{
	m_PerObjectData[i] = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	*(m_PerObjectData[i]) = PerObject3D();

	m_PerObjectConstantBuffer[i] = GetRenderDevice()->CreateConstantBuffer(PerObject3D());
}


for (int i = 0; i < m_PerObjectConstantBuffer.size(); i++)
{
	_aligned_free(m_PerObjectData[i]);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer[i]);
}

class CRenderDeviceLocker
{
public:
	CRenderDeviceLocker(const IRenderDevice*& RenderDevice)
		: m_RenderDevice(RenderDevice)
	{
		m_RenderDevice->Lock();
	}
	virtual ~CRenderDeviceLocker()
	{
		m_RenderDevice->Unlock();
	}

private:
	const IRenderDevice*& m_RenderDevice;
};


/*

#pragma omp parallel for
	for (int i = 0; i < static_cast<int>(m_RenderList.size()); i++)
	{
		const auto& it = m_RenderList[i];
		int threadNum = omp_get_thread_num();

		m_PerObjectData[threadNum]->Model = it.Node->GetWorldTransfom();
		m_PerObjectData[threadNum]->View = camera->GetViewMatrix();
		m_PerObjectData[threadNum]->Projection = camera->GetProjectionMatrix();

		ShaderMap shadersMap;
		if (it.Material)
			shadersMap = it.Material->GetShaders();
		if (shadersMap.empty())
			shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

		{
			CRenderDeviceLocker locker(GetRenderDevice());
			SetPerObjectConstantBufferData(threadNum);
		}

		{
			CRenderDeviceLocker locker(GetRenderDevice());
			it.Material->Bind(shadersMap);
			it.Geometry->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer(threadNum), shadersMap, it.Material, it.GeometryDrawArgs);
			it.Material->Unbind(shadersMap);
		}
	}

	*/



	/*
	std::vector<PerObject3D> perObject;
	for (const auto& it : m_RenderList)
	{
		m_PerObjectData[0]->Model = it.Node->GetWorldTransfom();
		m_PerObjectData[0]->View = camera->GetViewMatrix();
		m_PerObjectData[0]->Projection = camera->GetProjectionMatrix();
		perObject.push_back(*m_PerObjectData[0]);
	}

	if (m_RenderList.size() > 0)
	{
		IStructuredBuffer* buffer = GetRenderDevice()->CreateStructuredBuffer(perObject);

		const auto& geom = m_RenderList[0];

		ShaderMap shadersMap;
		if (geom.Material)
			shadersMap = geom.Material->GetShaders();
		if (shadersMap.empty())
			shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

		geom.Material->Bind(shadersMap);
		geom.Geometry->RenderInstanced(GetRenderEventArgs(), buffer, shadersMap, geom.Material, geom.GeometryDrawArgs);
		geom.Material->Unbind(shadersMap);

		GetRenderDevice()->DestroyStructuredBuffer(buffer);
	}
	*/

#endif




BuildRenderListPass::BuildRenderListPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: ScenePass(RenderDevice, Scene)
{}

BuildRenderListPass::~BuildRenderListPass()
{}



//
// BuildRenderListPass
//
const std::vector<BuildRenderListPass::SModelElement>& BuildRenderListPass::GetModelsList() const
{
	return m_ModelsList;
}

const std::vector<BuildRenderListPass::SGeometryElement>& BuildRenderListPass::GetGeometryList() const
{
	return m_GeometryList;
}

const std::vector<BuildRenderListPass::SLightElement>& BuildRenderListPass::GetLightList() const
{
	return m_LightList;
}



//
// IRenderPass
//
void BuildRenderListPass::PreRender(RenderEventArgs & e)
{
	ScenePass::PreRender(e);

	m_NodesList.clear();
	m_ModelsList.clear();
	m_GeometryList.clear();

	m_LightList.clear();
}

void BuildRenderListPass::Render(RenderEventArgs & e)
{
	ScenePass::Render(e);
}



//
// IVisitor
//
bool BuildRenderListPass::Visit(const ISceneNode3D * node)
{
	m_NodesList.push_back(node);
	return true;
}

bool BuildRenderListPass::Visit(const IModel * Model)
{
	_ASSERT(m_NodesList.empty() == false);
	m_ModelsList.push_back(SModelElement(m_NodesList.back(), Model));
	return true;
}

bool BuildRenderListPass::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	_ASSERT(m_ModelsList.empty() == false);
	m_GeometryList.push_back(SGeometryElement(m_ModelsList.back().Node, m_ModelsList.back().Model, Geometry, Material, GeometryDrawArgs));
	return true;
}

bool BuildRenderListPass::Visit(const ILightComponent3D * light)
{
	m_LightList.push_back(SLightElement(m_NodesList.back(), light));
	return true;
}



//
// CSceneNodeListPass
//
CSceneNodeListPass::CSceneNodeListPass(IRenderDevice & RenderDevice, const std::shared_ptr<IScene>& Scene)
	: ScenePass(RenderDevice, Scene)
{
}

CSceneNodeListPass::~CSceneNodeListPass()
{
}

const std::vector<const ISceneNode3D*>& CSceneNodeListPass::GetNodesList(SceneNodeType SceneNodeType) const
{
	if (m_NodesByType.find(SceneNodeType) == m_NodesByType.end())
		return m_EmptyList;
	return m_NodesByType.at(SceneNodeType);
}



//
// IRenderPass
//
void CSceneNodeListPass::PreRender(RenderEventArgs & e)
{
	ScenePass::PreRender(e);

	m_NodesByType.clear();
}

void CSceneNodeListPass::Render(RenderEventArgs & e)
{
	ScenePass::Render(e);
}



//
// IVisitor
//
bool CSceneNodeListPass::Visit(const ISceneNode3D * SceneNode)
{
	SceneNodeType type = SceneNode->GetType();
	if (SceneNode->GetType() < 0)
		return false;

	m_NodesByType[type].push_back(SceneNode);
	return false;
}
