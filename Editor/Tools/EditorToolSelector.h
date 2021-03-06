#pragma once

#include "EditorToolBase.h"
#include "Editor3D/EditorPasses/DrawSelectionPass.h"

class CEditorToolSelector
	: public IEditorToolSelector
	, public CEditorToolBase
{
public:
	CEditorToolSelector(IEditor& Editor);
	virtual ~CEditorToolSelector();

	// IEditorTool
	void Enable() override;
	void Disable() override;

	// IEditorToolSelector
	void SelectNode(std::shared_ptr<ISceneNode> Node) override;
	void SelectNodes(const std::vector<std::shared_ptr<ISceneNode>>& Nodes) override;
	void ClearSelection() override;
	void AddNode(std::shared_ptr<ISceneNode> Node) override;
	void RemoveNode(std::shared_ptr<ISceneNode> Node) override;
	std::shared_ptr<ISceneNode> GetFirstSelectedNode() const override;
	bool IsNodeSelected(std::shared_ptr<ISceneNode> Node) const override;
	const SelectedNodes& GetSelectedNodes() const override;

	// CEditorToolBase
	void DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget) override;
	bool OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseReleased(const MouseButtonEventArgs & e, const Ray& RayToWorld) override;
	void OnMouseMoved(const MouseMotionEventArgs& e, const Ray& RayToWorld) override;

	// IEditorToolUI
	void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) override;

protected:
	void RaiseSelectEvent();

private:
	SelectedNodes m_SelectedNodes;
	IEditorToolSelectorEventListener& m_NodesSelectorEventListener;

	bool m_IsSelecting2D;
	glm::vec2 m_SelectionPrevPos;
	std::shared_ptr<IUIControlCommon> m_SelectionTexture;
	std::shared_ptr<CDrawSelectionPass> m_DrawSelectionPass;
};
