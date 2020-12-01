#include "stdafx.h"

// General
#include "BaseTypedList3DPass.h"

CBaseList3DPass::CBaseList3DPass(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypedListsPass>& SceneNodeListPass, ObjectClass SceneNodeType)
	: RenderPassPipelined(RenderDevice)
	, m_SceneNodeListPass(SceneNodeListPass)
	, m_PerObjectParameter(nullptr)
{
	m_AcceptSceneNodeTypes.push_back(SceneNodeType);
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

CBaseList3DPass::CBaseList3DPass(IRenderDevice & RenderDevice, const std::shared_ptr<CSceneCreateTypedListsPass>& SceneNodeListPass, std::vector<ObjectClass> SceneNodeTypesList)
	: RenderPassPipelined(RenderDevice)
	, m_SceneNodeListPass(SceneNodeListPass)
	, m_PerObjectParameter(nullptr)
{
	m_AcceptSceneNodeTypes = SceneNodeTypesList;
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

CBaseList3DPass::~CBaseList3DPass()
{
}

//
// IRenderPassPipelined
//
void CBaseList3DPass::Render(RenderEventArgs & e)
{
	/*const auto& acceptableNodeTypesList = GetAcceptableNodeTypes();
	std::for_each(acceptableNodeTypesList.begin(), acceptableNodeTypesList.end(), [this](SceneNodeType acceptableNodeType) {
		if (GetSceneNodeListPass()->HasNodesList(acceptableNodeType))
		{
			const auto& list = GetSceneNodeListPass()->GetNodesList(acceptableNodeType);
			std::for_each(list.begin(), list.end(), [this](const CSceneCreateTypelessListPass::SNodeElement& SceneNodeElement) {
				if (Visit(SceneNodeElement.SceneNode))
				{
					const auto& components = SceneNodeElement.SceneNode->GetComponents();
					std::for_each(components.begin(), components.end(), [this](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component) {
						Component.second->Accept(this);
					});
				}
			});
		}
	});*/

	for (const auto& acceptableNodeType : GetAcceptableNodeTypes())
	{
		if (GetSceneNodeListPass()->HasGeometriesList(acceptableNodeType))
		{
			for (const auto& SceneNodeElement : GetSceneNodeListPass()->GetGeometriesList(acceptableNodeType))
			{
				DoRenderSceneNode(SceneNodeElement.Node);
				DoRenderGeometry(SceneNodeElement.Geometry, SceneNodeElement.Material , SceneNodeElement.GeometryDrawArgs);
			}
		}
	}
}

//
// IVisitor
//
void CBaseList3DPass::DoRenderSceneNode(const ISceneNode * SceneNode)
{
	PerObject perObject;
	perObject.Model = SceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject);

	if (m_PerObjectParameter == nullptr)
		m_PerObjectParameter = (GetPipeline().GetVertexShaderPtr()->GetShaderParameterByName("PerObject"));

	if (m_PerObjectParameter && m_PerObjectConstantBuffer != nullptr)
	{
		m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
		m_PerObjectParameter->Bind();
	}
}

void CBaseList3DPass::DoRenderModel(const IModel * Model)
{
	Model->Render();
}

void CBaseList3DPass::DoRenderGeometry(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	if (Material)
		Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);

	if (Material)
		Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
}



//
// Protected
//
const std::shared_ptr<CSceneCreateTypedListsPass>& CBaseList3DPass::GetSceneNodeListPass() const
{
	return m_SceneNodeListPass;
}

const std::vector<ObjectClass>& CBaseList3DPass::GetAcceptableNodeTypes() const
{
	return m_AcceptSceneNodeTypes;
}
