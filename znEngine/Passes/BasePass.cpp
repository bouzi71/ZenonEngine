#include <stdafx.h>

// Include
#include <Application.h>
#include <SceneFunctional//Scene3D.h>
#include <SceneFunctional//SceneNode3D.h>

// General
#include "BasePass.h"

BasePass::BasePass()
	: m_pRenderEventArgs(nullptr)
	, m_RenderDevice(_RenderDevice)
{

}

BasePass::BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: m_pRenderEventArgs(nullptr)
	, m_Scene(scene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{

}

BasePass::~BasePass()
{
}

void BasePass::PreRender(Render3DEventArgs& e)
{
	e.PipelineState = m_Pipeline.get();
	SetRenderEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}
}

void BasePass::Render(Render3DEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(*this);
	}
}

void BasePass::PostRender(Render3DEventArgs& e)
{
	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}



//
// Inherited from Visitor
//
bool BasePass::Visit(std::shared_ptr<SceneNode3D> node)
{
	m_pRenderEventArgs->Node = node.operator->();

	const Camera* camera = GetRenderEventArgs().Camera;
	if (camera)
	{
		node->UpdateCamera(camera);

		PerObject perObjectData;
		perObjectData.Model               = node->GetComponent<CTransformComponent>()->GetWorldTransfom();
		perObjectData.ModelView           = camera->GetViewMatrix()       * perObjectData.Model;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;

		perObjectData.View                = camera->GetViewMatrix();
		perObjectData.Projection          = camera->GetProjectionMatrix();

		// Update the constant buffer data
		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}

bool BasePass::Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (m_pRenderEventArgs)
	{
		return Mesh->Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
	}

	return false;
}

bool BasePass::Visit(std::shared_ptr<CLight3D> light)
{
	return false;
}



//
// Update viewport (need for texture resizing)
//
void BasePass::UpdateViewport(Viewport _viewport)
{
	m_Pipeline->GetRasterizerState().SetViewport(_viewport);
}

//----------------------------------------------------------------------

void BasePass::SetRenderEventArgs(Render3DEventArgs& e)
{
	m_pRenderEventArgs = &e;
}

Render3DEventArgs& BasePass::GetRenderEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

//----------------------------------------------------------------------

std::shared_ptr<IRenderDevice> BasePass::GetRenderDevice() const
{
	return m_RenderDevice.lock();
}

std::shared_ptr<PipelineState> BasePass::GetPipelineState() const
{
	return m_Pipeline;
}
