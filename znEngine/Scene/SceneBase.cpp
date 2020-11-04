#include "stdafx.h"

// General
#include "SceneBase.h"

SceneBase::SceneBase(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_RenderWindow(RenderWindow)
	, m_Finder(*this)
{
	
}

SceneBase::~SceneBase()
{}




//
// IScene
//
IBaseManager& SceneBase::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& SceneBase::GetRenderDevice() const
{
	return m_RenderDevice;
}

IRenderWindow& SceneBase::GetRenderWindow() const
{
	return m_RenderWindow;
}

const ISceneFinder& SceneBase::GetFinder() const
{
	return m_Finder;
}


void SceneBase::Initialize()
{
	m_VideoSettings = GetBaseManager().GetManager<ISettings>()->GetGroup("Video");

	m_FrameQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::Timer, 1);
	m_TestQuery = GetRenderDevice().GetObjectsFactory().CreateQuery(IQuery::QueryType::CountSamples, 1);

	m_RootSceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
	m_RootSceneNode->SetName("RootSceneNode");
	std::dynamic_pointer_cast<ISceneNodeInternal>(m_RootSceneNode)->SetPersistanceInternal(true);

	m_RootUIControl = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI, *this);
	m_RootUIControl->SetName("RootUIControl");

	{
		m_CameraPosText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_CameraPosText);
		m_CameraPosText->SetTranslate(glm::vec2(5.0f, 5.0f));

		m_CameraRotText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_CameraRotText);
		m_CameraRotText->SetTranslate(glm::vec2(5.0f, 25.0f));

		m_CameraRot2Text = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_CameraRot2Text);
		m_CameraRot2Text->SetTranslate(glm::vec2(5.0f, 45.0f));

		m_FPSText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_FPSText);
		m_FPSText->SetTranslate(glm::vec2(5.0f, 65.0f));

		m_StatisticUpdateText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_StatisticUpdateText);
		m_StatisticUpdateText->SetTranslate(glm::vec2(5.0f, 85.0f));

		m_StatisticPreRenderText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_StatisticPreRenderText);
		m_StatisticPreRenderText->SetTranslate(glm::vec2(5.0f, 105.0f));

		m_StatisticRenderText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_StatisticRenderText);
		m_StatisticRenderText->SetTranslate(glm::vec2(5.0f, 125.0f));

		m_StatisticPostRenderText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_StatisticPostRenderText);
		m_StatisticPostRenderText->SetTranslate(glm::vec2(5.0f, 145.0f));

		m_StatisticRenderUIText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_StatisticRenderUIText);
		m_StatisticRenderUIText->SetTranslate(glm::vec2(5.0f, 165.0f));

		m_StatisticSummaText = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cSceneNodeUI_Text, *this);
		GetRootUIControl()->AddChild(m_StatisticSummaText);
		m_StatisticSummaText->SetTranslate(glm::vec2(5.0f, 185.0f));
	}


}

void SceneBase::Finalize()
{
}


void SceneBase::AddEventListener(std::shared_ptr<ISceneEventsListener> Listener)
{
	const auto& it = std::find(m_EventListeners.begin(), m_EventListeners.end(), Listener);
	if (it != m_EventListeners.end())
	{
		Log::Error("Scene: Unable to add 'ISceneEventsListener' because already exists.");
		return;
	}
	m_EventListeners.push_back(Listener);
}

void SceneBase::RemoveEventListener(std::shared_ptr<ISceneEventsListener> Listener)
{
	const auto& it = std::find(m_EventListeners.begin(), m_EventListeners.end(), Listener);
	if (it == m_EventListeners.end())
	{
		Log::Error("Scene: Unable to remove 'ISceneEventsListener' because non exists.");
		return;
	}
	m_EventListeners.erase(it);
}



//
// IScene
//
std::shared_ptr<ISceneNode> SceneBase::GetRootSceneNode() const
{
	return m_RootSceneNode;
}

std::shared_ptr<IUIControl> SceneBase::GetRootUIControl() const
{
	return m_RootUIControl;
}

void SceneBase::SetRenderer(std::shared_ptr<IRenderer> Renderer)
{
	m_Renderer = Renderer;
}

std::shared_ptr<IRenderer> SceneBase::GetRenderer() const
{
	return m_Renderer;
}

void SceneBase::SetCameraController(std::shared_ptr<ICameraController> CameraController)
{
	_ASSERT(CameraController != nullptr);
	m_CameraController = CameraController;
}

std::shared_ptr<ICameraController> SceneBase::GetCameraController() const
{
	return m_CameraController;
}




void SceneBase::Accept(IVisitor * visitor)
{
	std::lock_guard<std::mutex> lock(m_ChildModifyLock);

	if (GetRootSceneNode())
		GetRootSceneNode()->Accept(visitor);

	if (GetRootUIControl())
		GetRootUIControl()->Accept(visitor);
}



//
// ISceneInternal
//
void SceneBase::AddChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Warn("Can't add child instanse to nullptr parent.");
		return;
	}

	if (m_ChildModifyLock.try_lock())
	{
		try
		{
			std::dynamic_pointer_cast<ISceneNodeInternal>(ParentNode)->AddChildInternal(ChildNode);
		}
		catch (...)
		{
			m_ChildModifyLock.unlock();
			throw;
		}

		m_ChildModifyLock.unlock();
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);
		m_AddChildList.push_back(std::make_pair(ParentNode, ChildNode));
	}
}

void SceneBase::RemoveChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (ParentNode == nullptr)
	{
		Log::Error("Can't remove child node '%s' from nullptr parent.", ChildNode->GetName().c_str());
		return;
	}

	if (m_ChildModifyLock.try_lock())
	{
		try
		{
			std::dynamic_pointer_cast<ISceneNodeInternal>(ParentNode)->RemoveChildInternal(ChildNode);
		}
		catch (...)
		{
			m_ChildModifyLock.unlock();
			throw;
		}

		m_ChildModifyLock.unlock();
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_ListsAreBusy);
		m_RemoveChildList.push_back(std::make_pair(ParentNode, ChildNode));
	}
}

void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& OwnerNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	//m_SceneChangeEvent(SceneChangeEventArgs(this, SceneChangeType, OwnerNode, ChildNode));
}



//
// IRenderWindowEventListener
//
void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (GetCameraController())
	{
		GetCameraController()->OnUpdate(e);

		e.Camera           = GetCameraController()->GetCamera().get();
		e.CameraForCulling = GetCameraController()->GetCamera().get();
	}


	std::lock_guard<std::mutex> lock(m_ChildModifyLock);
	std::lock_guard<std::mutex> lock2(m_ListsAreBusy);

	for (const auto& it : m_AddChildList)
		it.first->AddChild(it.second);
	m_AddChildList.clear();

	for (const auto& it : m_RemoveChildList)
		it.first->RemoveChild(it.second);
	m_RemoveChildList.clear();


	if (GetRootSceneNode())
		DoUpdate_Rec(GetRootSceneNode(), e);
}

void SceneBase::OnPreRender(RenderEventArgs & e)
{
	m_Start = std::chrono::high_resolution_clock::now();
}

void SceneBase::OnRender(RenderEventArgs & e)
{
	if (auto cameraController = GetCameraController())
	{
		e.Camera = cameraController->GetCamera().get();
		e.CameraForCulling = cameraController->GetCamera().get();
	}
	else
		_ASSERT(false);

	if (auto renderer = GetRenderer())
		renderer->Render3D(e);
}

void SceneBase::OnPostRender(RenderEventArgs & e)
{
	if (auto cameraController = GetCameraController())
	{
		glm::vec3 cameraTrans = cameraController->GetCamera()->GetTranslation();
		m_CameraPosText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: x = " + std::to_string(cameraTrans.x) + ", y = " + std::to_string(cameraTrans.y) + ", z = " + std::to_string(cameraTrans.z));
		m_CameraRotText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: yaw = " + std::to_string(cameraController->GetCamera()->GetYaw()) + ", pitch = " + std::to_string(cameraController->GetCamera()->GetPitch()));
		m_CameraRot2Text->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: [" + std::to_string(cameraController->GetCamera()->GetDirection().x) + ", " + std::to_string(cameraController->GetCamera()->GetDirection().y) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().z) + "].");
	}

	e.Camera = nullptr;
	e.CameraForCulling = nullptr;
}

void SceneBase::OnRenderUI(RenderEventArgs & e)
{
	m_End = std::chrono::high_resolution_clock::now();

	if (auto cameraController = GetCameraController())
	{
		e.Camera = cameraController->GetCamera().get();
		e.CameraForCulling = cameraController->GetCamera().get();
	}

	if (auto renderer = GetRenderer())
		renderer->RenderUI(e);

	{
		/*IQuery::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter);
		if (frameResult.IsValid)
		{
			if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
				m_FrameTime = frameResult.DeltaTime * 1000.0;
			else
				m_FrameTime = frameResult.DeltaTime / 1000000.0;

			double fpsValue = 1000.0f / m_FrameTime;

			m_FPSText->GetProperties()->GetPropertyT<std::string>("Text")->Set("FPS: " + std::to_string(uint64(fpsValue)));
		}*/

		//int elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(m_End - m_Start).count();
		double fpsValue = 1000.0 / double(e.DeltaTime);
		m_FPSText->GetProperties()->GetPropertyT<std::string>("Text")->Set("FPS: " + std::to_string(uint64(fpsValue)));


		m_StatisticUpdateText->GetProperties()    ->GetPropertyT<std::string>("Text")->Set("W Update   : " + std::to_string(GetRenderWindow().GetUpdateDeltaTime()));
		m_StatisticPreRenderText->GetProperties() ->GetPropertyT<std::string>("Text")->Set("W PreRender: " + std::to_string(GetRenderWindow().GetPreRenderDeltaTime()));
		m_StatisticRenderText->GetProperties()    ->GetPropertyT<std::string>("Text")->Set("W Render   : " + std::to_string(GetRenderWindow().GetRenderDeltaTime()));
		m_StatisticPostRenderText->GetProperties()->GetPropertyT<std::string>("Text")->Set("W PostRen  : " + std::to_string(GetRenderWindow().GetPostRenderDeltaTime()));
		m_StatisticRenderUIText->GetProperties()  ->GetPropertyT<std::string>("Text")->Set("W RenderUI : " + std::to_string(GetRenderWindow().GetRenderUIDeltaTime()));
		m_StatisticSummaText->GetProperties()     ->GetPropertyT<std::string>("Text")->Set("W          : " + std::to_string(GetRenderWindow().GetSummaDeltaTime()));
	}
}



//
// INativeWindowEventListener
//
void SceneBase::OnWindowResize(ResizeEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnResize(e);

	if (m_Renderer)
		m_Renderer->Resize(e.Width, e.Height);
}

// Keyboard events

bool SceneBase::OnWindowKeyPressed(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyPressed(e);

	if (GetRootUIControl())
		return DoKeyPressed_Rec(GetRootUIControl(), e);

	return false;
}

void SceneBase::OnWindowKeyReleased(KeyEventArgs & e)
{
	if (GetCameraController())
		GetCameraController()->OnKeyReleased(e);

	if (GetRootUIControl())
		DoKeyReleased_Rec(GetRootUIControl(), e);
}

// Mouse events

void SceneBase::OnWindowMouseMoved(MouseMotionEventArgs & e)
{
	OnMouseMoved(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint()));

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseMoved(e);

	if (GetRootUIControl())
		DoMouseMoved_Rec(GetRootUIControl(), e);
}

bool SceneBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & e)
{	
	if (OnMousePressed(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint())))
		return true;

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseButtonPressed(e);

	if (GetRootUIControl())
		return DoMouseButtonPressed_Rec(GetRootUIControl(), e);

	return false;
}

void SceneBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & e)
{
	OnMouseReleased(e, GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), e.GetPoint()));

	if (GetCameraController() == nullptr)
		throw CException("You must set CameraController to scene!");
	GetCameraController()->OnMouseButtonReleased(e);

	if (GetRootUIControl())
		DoMouseButtonReleased_Rec(GetRootUIControl(), e);
}

bool SceneBase::OnWindowMouseWheel(MouseWheelEventArgs & e)
{
	if (GetCameraController())
		if (GetCameraController()->OnMouseWheel(e))
			return true;

	if (GetRootUIControl())
		if (DoMouseWheel_Rec(GetRootUIControl(), e))
			return true;

	return false;
}



//
//
//
void SceneBase::LoadFromFile(const std::string& FileName)
{
	auto file = GetBaseManager().GetManager<IFilesManager>()->Open(FileName);
	if (file == nullptr)
		throw CException("Scene file '%s' not found.", FileName.c_str());

	ResetScene();

	auto root = GetRootSceneNode();

	CXMLManager xml(GetBaseManager());
	auto xmlReader = xml.CreateReader(file);
	auto xmlRootChild = xmlReader->GetChilds()[0];

	auto tempFakeRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, root);
	tempFakeRoot->SetName("TempFakeRoot");

	auto newRootNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(xmlRootChild, *this, tempFakeRoot);


	// Update persistance nodes
	for (const auto& existingRootChild : root->GetChilds())
	{
		if (existingRootChild->IsPersistance())
		{
			auto xmlChild = newRootNode->GetChild(existingRootChild->GetName());
			if (xmlChild != nullptr)
			{
				xmlChild->CopyTo(existingRootChild);

				// To delete persistance node, we must clear this flag
				std::dynamic_pointer_cast<ISceneNodeInternal>(xmlChild)->SetPersistanceInternal(false);
				newRootNode->RemoveChild(xmlChild);
			}
		}
	}

	// Add new childs
	while (false == newRootNode->GetChilds().empty())
	{
		auto newRootChild = *(newRootNode->GetChilds().begin());
		root->AddChild(newRootChild);
	}

	tempFakeRoot->MakeMeOrphan();
}

void SceneBase::SaveToFile(const std::string& FileName) const
{
	CXMLManager manager(GetBaseManager());

	auto rootWriter = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->SaveSceneNode3DXML(GetRootSceneNode());

	auto xml = manager.CreateWriter();
	xml->AddChild(rootWriter);

	auto file = manager.SaveWriterToFile(xml, FileName);
	file->Save();
}

void SceneBase::ResetScene()
{
	auto root = GetRootSceneNode();

	// Remove existing childs
	Log::Info("ResetScene: Root contains '%d' childs BEFORE reset.", root->GetChilds().size());
	size_t mustLeaveExisting = 0;
	while (root->GetChilds().size() > mustLeaveExisting)
	{
		auto editorChild = *(root->GetChilds().begin() + mustLeaveExisting);
		if (editorChild->IsPersistance())
		{
			mustLeaveExisting++;
			Log::Info("ResetScene: Child '%s' was not removed, because is persistant.", editorChild->GetName().c_str());
			continue;
		}

		root->RemoveChild(editorChild);
	}
	Log::Info("ResetScene: Root contains '%d' childs AFTER reset.", root->GetChilds().size());
}




//
// Protected
//
bool SceneBase::OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld)
{
	return false;
}

void SceneBase::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
}

void SceneBase::OnMouseMoved(const MouseMotionEventArgs & e, const Ray& RayToWorld)
{

}



//
// Protected
//
void SceneBase::DoUpdate_Rec(const std::shared_ptr<ISceneNode>& Node, const UpdateEventArgs & e)
{
	Node->Update(e);

	const auto& components = Node->GetComponents();
	std::for_each(components.begin(), components.end(), [&e](const std::pair<ObjectClass, std::shared_ptr<ISceneNodeComponent>>& Component)
	{
		_ASSERT(Component.second);
		Component.second->Update(e);
	});

	const auto& childs = Node->GetChilds();
	std::for_each(childs.begin(), childs.end(), [this, &e](const std::shared_ptr<ISceneNode>& Child)
	{
		if (Child != nullptr)
			DoUpdate_Rec(Child, e);
	});
}

bool SceneBase::DoKeyPressed_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			if (DoKeyPressed_Rec(child, e))
				return true;

		if (NodeAsUINode->OnKeyPressed(e))
			return true;
	}

	return false;
}

void SceneBase::DoKeyReleased_Rec(const std::shared_ptr<IUIControl>& Node, KeyEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			DoKeyReleased_Rec(child, e);
		}

		NodeAsUINode->OnKeyReleased(e);
	}
}

void SceneBase::DoMouseMoved_Rec(const std::shared_ptr<IUIControl>& Node, MouseMotionEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
		{
			DoMouseMoved_Rec(child, e);
		}

		NodeAsUINode->OnMouseMoved(e);

		// Synteric events impl
		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
		{
			if (!NodeAsUINode->IsMouseOnNode())
			{
				NodeAsUINode->OnMouseEntered();
				NodeAsUINode->DoMouseEntered();
			}
		}
		else
		{
			if (NodeAsUINode->IsMouseOnNode())
			{
				NodeAsUINode->OnMouseLeaved();
				NodeAsUINode->DoMouseLeaved();
			}
		}
	}
}

bool SceneBase::DoMouseButtonPressed_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			if (DoMouseButtonPressed_Rec(child, e))
				return true;

		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
			if (NodeAsUINode->OnMouseButtonPressed(e))
				return true;
	}

	return false;
}

void SceneBase::DoMouseButtonReleased_Rec(const std::shared_ptr<IUIControl>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			DoMouseButtonReleased_Rec(child, e);

		NodeAsUINode->OnMouseButtonReleased(e);
	}
}

bool SceneBase::DoMouseWheel_Rec(const std::shared_ptr<IUIControl>& Node, MouseWheelEventArgs & e)
{
	std::shared_ptr<CUIControl> NodeAsUINode = std::dynamic_pointer_cast<CUIControl>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			if (DoMouseWheel_Rec(child, e))
				return true;
		}

		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
			if (NodeAsUINode->OnMouseWheel(e))
				return true;
	}

	return false;
}