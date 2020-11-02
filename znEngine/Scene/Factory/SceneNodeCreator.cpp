#include "stdafx.h"

// General
#include "SceneNodeCreator.h"

// Additional
#include "Scene/Nodes/UIText.h"
#include "Scene/Nodes/UIColor.h"
#include "Scene/RTSSceneNodeGround.h"

CSceneNodeEngineCreator::CSceneNodeEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("CSceneNode", cSceneNode3D);
	AddKey("RTSSceneNodeGround", cCRTSSceneNodeGround);
}

CSceneNodeEngineCreator::~CSceneNodeEngineCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto sceneNodeCreationArgs = static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs);
	
	// 1. Create object
	std::shared_ptr<ISceneNode> createdNode = nullptr;
	if (Index == 0)
	{
		createdNode = sceneNodeCreationArgs->GetScene().CreateSceneNode3DInternal<CSceneNode>();
	}
	else if (Index == 1)
	{
		createdNode = sceneNodeCreationArgs->GetScene().CreateSceneNode3DInternal<CRTSSceneNodeGround>();
	}

	// 2. Check
	if (createdNode == nullptr)
		throw CException("CSceneNodeEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	// 3. Assign GUID
	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNode))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNode3DEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	// 4. SceneNode specific
	auto& scene = sceneNodeCreationArgs->GetScene();
	auto parent = sceneNodeCreationArgs->GetParent();
	if (parent)
		scene.AddChild(parent, createdNode);
	else if (scene.GetRootNode3D())
		scene.AddChild(scene.GetRootNode3D(), createdNode);

	return createdNode;
}



//
// CSceneNodeUIEngineCreator
//
CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("SceneNodeUI", cSceneNodeUI);
	AddKey("SceneNodeUIText", cSceneNodeUI_Text);
	AddKey("SceneNodeUIColor", cSceneNodeUI_Color);
}

CSceneNodeUIEngineCreator::~CSceneNodeUIEngineCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeUIEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto sceneNodeCreationArgs = static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs);

	std::shared_ptr<IUIControl> createdNode = nullptr;
	if (Index == 0)
	{
		createdNode = sceneNodeCreationArgs->GetScene().CreateSceneNodeUIInternal<SceneNodeUI>();
	}
	else if (Index == 1)
	{
		createdNode = sceneNodeCreationArgs->GetScene().CreateSceneNodeUIInternal<CUITextNode>();
	}
	else if (Index == 2)
	{
		createdNode = sceneNodeCreationArgs->GetScene().CreateSceneNodeUIInternal<CUIColorNode>();
	}

	if (createdNode == nullptr)
		throw CException("SceneNodeUIEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNode))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNodeUIEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	auto& scene = sceneNodeCreationArgs->GetScene();
	auto parent = sceneNodeCreationArgs->GetParent();
	if (parent)
		parent->AddChild(createdNode);
	else if (scene.GetRootNodeUI())
		scene.GetRootNodeUI()->AddChild(createdNode);
	else
		_ASSERT(true); // RootNode

	return createdNode;
}
