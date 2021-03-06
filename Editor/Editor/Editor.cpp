#include "stdafx.h"

// General
#include "Editor.h"

// Additional
#include "EditorUI/EditorUIFrame.h"
#include "Editor3D/Editor3DFrame.h"

CEditor::CEditor(IBaseManager & BaseManager)
	: m_BaseManager(BaseManager)
	, m_EditorUIFrame(nullptr)
	, m_Editor3DFrame(nullptr)
	, m_Editor3DPreviewFrame(nullptr)
	, m_Tools(*this)
	, m_EditorShell(*this)
{

}

CEditor::~CEditor()
{
}

IBaseManager & CEditor::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice & CEditor::GetRenderDevice() const
{
	return m_BaseManager.GetApplication().GetRenderDevice();
}

IEditorUIFrame& CEditor::GetUIFrame() const
{
	_ASSERT(m_EditorUIFrame != nullptr);
	return *m_EditorUIFrame;
}

IEditor3DFrame& CEditor::Get3DFrame() const
{
	_ASSERT(m_Editor3DFrame != nullptr);
	return *m_Editor3DFrame;
}

IEditor3DPreviewFrame & CEditor::Get3DPreviewFrame() const
{
	_ASSERT(m_Editor3DPreviewFrame != nullptr);
	return *m_Editor3DPreviewFrame;
}

IEditorTools& CEditor::GetTools()
{
	return m_Tools;
}

IEditorShell & CEditor::GetShell()
{
	return m_EditorShell;
}

bool CEditor::IsNodeSelected(std::shared_ptr<ISceneNode> Node) const
{
	return m_Tools.GetToolTConst<IEditorToolSelector>(ETool::EToolSelector).IsNodeSelected(Node);
}

std::shared_ptr<ISceneNode> CEditor::GetFirstSelectedNode() const
{
	return m_Tools.GetToolTConst<IEditorToolSelector>(ETool::EToolSelector).GetFirstSelectedNode();
}

std::vector<std::shared_ptr<ISceneNode>> CEditor::GetSelectedNodes() const
{
	std::vector<std::shared_ptr<ISceneNode>> nodes;
	for (const auto& nW : m_Tools.GetToolTConst<IEditorToolSelector>(ETool::EToolSelector).GetSelectedNodes())
		if (auto locked = nW.lock())
			nodes.push_back(locked);
	return nodes;
}



//
// IEditorToolSelectorEventListener
//
void CEditor::OnSelectNode()
{
	dynamic_cast<IEditorToolSelectorEventListener*>(m_Editor3DFrame)->OnSelectNode();
	dynamic_cast<IEditorToolSelectorEventListener*>(m_EditorUIFrame)->OnSelectNode();
}
