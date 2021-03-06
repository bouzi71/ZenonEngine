#include "stdafx.h"

// General
#include "EditorToolRotatorRTS.h"

// Additional
#include "Editor3D/EditorMaterials/MaterialEditorTool.h"

CEditorToolRotatorRTS::CEditorToolRotatorRTS(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_RotatorNumber(EMoverDirection::None)
	, m_InitialRotationDegrees(0.0f)
	, m_StartMousePosY(0)
{
}

CEditorToolRotatorRTS::~CEditorToolRotatorRTS()
{
}


void CEditorToolRotatorRTS::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
		OnNodeSelected(node);
}

void CEditorToolRotatorRTS::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn->setChecked(IsEnabled());

	Clear();
	m_RotatingNode.reset();
	m_RotatorRoot->SetPosition(glm::vec3(Math::MinFloat));
}

void CEditorToolRotatorRTS::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_RotatorRoot = GetScene().CreateSceneNodeT<ISceneNode>();
	m_RotatorRoot->SetName("RotatorRTSRoot");

	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateTorus(1.0f, 0.05f);

	auto materialY = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialY->SetColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	m_RotatorY = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
	m_RotatorY->SetName("RotatorRTSY");
	m_RotatorY->GetComponentT<IModelComponent>()->SetModel(modelY);
}

bool CEditorToolRotatorRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
	{
		Clear();
		return false;
	}

	_ASSERT(false == IsChildOf(m_RotatorRoot, rotatingNode));

	auto nodes = GetScene().GetFinder().FindIntersection(RayToWorld, nullptr, m_RotatorRoot);
	if (nodes.empty())
		return false;


	for (const auto& it : nodes)
	{
		if (it.second == m_RotatorY)
			m_RotatorNumber = EMoverDirection::Y;
	}

	if (m_RotatorNumber != EMoverDirection::None)
	{
		m_InitialRotationDegrees = rotatingNode->GetLocalRotationEuler().y;
		m_StartMousePosY = e.Y;
		return true;
	}

	return false;
}

void CEditorToolRotatorRTS::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_RotatorNumber == EMoverDirection::None)
		return;
	Clear();
}

void CEditorToolRotatorRTS::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (m_RotatorNumber == EMoverDirection::None)
		return;
	
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
	{
		Clear();
		return;
	}

	float rotatorInitialAngleDegrees = m_InitialRotationDegrees + float(m_StartMousePosY - e.Y) / glm::pi<float>();
	rotatorInitialAngleDegrees = GetEditor().GetUIFrame().FixRotatorCoords(rotatorInitialAngleDegrees);
	rotatingNode->SetLocalRotationEuler(glm::vec3(0.0f, rotatorInitialAngleDegrees, 0.0f));

	// Refresh selection bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(rotatingNode);
}

void CEditorToolRotatorRTS::OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode)
{
	m_RotatingNode = SelectedNode;
	m_RotatorRoot->SetPosition(SelectedNode->GetPosition());

	glm::vec3 scale(1.0f);
	if (auto collider = SelectedNode->GetComponentT<IColliderComponent>())
	{
		const auto& worldBounds = collider->GetWorldBounds();
		if (false == worldBounds.IsInfinite())
			scale = glm::vec3(worldBounds.getRadius());
	}

	m_RotatorRoot->SetScale(scale);
}



//
// IEditorToolUI
//
void CEditorToolRotatorRTS::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
	auto btn = QtUIFrame.getUI().editorToolRotatorRTSBtn;
	btn->connect(btn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolRotatorRTS);
	});
}



//
// Protected
//
void CEditorToolRotatorRTS::Clear()
{
	m_RotatorNumber = EMoverDirection::None;
}

std::shared_ptr<ISceneNode> CEditorToolRotatorRTS::GetRotatingNode()
{
	return m_RotatingNode.lock();
}
