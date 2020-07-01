#pragma once

#include <QMainWindow>
#include "ui_MainEditor.h"

#include "EditorInterfaces.h"
#include "SceneNodeTreeModel.h"

#include "PropertyEditor/PropertiesController.h"

class MainEditor 
	: public QMainWindow
	, public IEditorUIFrame
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	// IEditorUIFrame
	void OnSceneNode3DSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D) const override;

	void OnSceneNodeSelected(ISceneNode3D* SceneNode);

	void ApplyScene(std::shared_ptr<IScene> Scene);
	void ApplyTest();

	Ui::MainEditorClass getUI() const { return ui; }

private slots:
	void onCustomContextMenu(const QPoint& point);
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
	CSceneNodeTreeModel * m_SceneTreeViewerModel;
	std::shared_ptr<IScene> m_Scene;
	

	std::shared_ptr<QMenu>                          m_SceneTreeViewerContextMenu;

	std::shared_ptr<CPropertiesController>          m_PropertiesController;

private:
	Ui::MainEditorClass ui;
};
