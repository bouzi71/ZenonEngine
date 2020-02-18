#include "stdafx.h"

// General
#include "RenderPass.h"

RenderPass::RenderPass(IRenderDevice& RenderDevice)
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice.GetBaseManager())
{}

RenderPass::~RenderPass()
{}



//
// IRenderPass
//
void RenderPass::SetEnabled(bool Value)
{
	m_Enabled = Value;
}

bool RenderPass::IsEnabled() const
{
	return m_Enabled;
}

void RenderPass::PreRender(RenderEventArgs& e)
{
	m_RenderEventArgs = &e;
}

void RenderPass::PostRender(RenderEventArgs& e)
{}



//
// IVisitor
//
bool RenderPass::Visit(const ISceneNode3D* node)
{
	return false;
}

bool RenderPass::Visit(const ISceneNodeUI* node)
{
	return false;
}

bool RenderPass::Visit(const IModel* Model)
{
    return false;
}

bool RenderPass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return false;
}

bool RenderPass::Visit(const ILightComponent3D* light)
{
	return false;
}



//
// Protected
//
const RenderEventArgs* RenderPass::GetRenderEventArgs() const
{
    _ASSERT(m_RenderEventArgs != nullptr);
    return m_RenderEventArgs;
}

IRenderDevice& RenderPass::GetRenderDevice() const
{
    return m_RenderDevice;
}

const IBaseManager& RenderPass::GetBaseManager() const
{
	return m_BaseManager;
}
