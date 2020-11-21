#include "stdafx.h"

// Gerenal
#include "Editor3DPreviewScene.h"


namespace
{
	void ExtendsBoundsRecursive(BoundingBox& Bounds, std::shared_ptr<ISceneNode> SceneNode)
	{
		if (auto collider = SceneNode->GetComponentT<IColliderComponent3D>())
		{
			const auto& colliderBounds = collider->GetBounds();
			if (false == colliderBounds.IsInfinite())
			{
				Bounds.makeUnion(colliderBounds);
			}
		}

		for (const auto& ch : SceneNode->GetChilds())
			ExtendsBoundsRecursive(Bounds, ch);
	}
}


CEditor3DPreviewScene::CEditor3DPreviewScene(IEditor& Editor, IRenderWindow& RenderWindow)
	: SceneBase(Editor.GetBaseManager(), RenderWindow)
{
	dynamic_cast<IEditorPrivate&>(Editor).Set3DPreviewFrame(this);
}

CEditor3DPreviewScene::~CEditor3DPreviewScene()
{
}



//
// IEditor3DPreviewFrame
//
void CEditor3DPreviewScene::SetSceneNode(std::shared_ptr<ISceneNode> SceneNode)
{
	_ASSERT(SceneNode != nullptr);

	Clean();

	auto copy = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<CSceneNodeFactory>()->CreateSceneNode3D(SceneNode->GetClass(), *this, m_SceneNode);
	SceneNode->CopyTo(copy);

	BoundingBox bbox;
	ExtendsBoundsRecursive(bbox, copy);
	if (bbox.IsInfinite())
	{
		bbox.setMin(glm::vec3(-0.5f));
		bbox.setMax(glm::vec3(0.5f));
	}

	float radius = bbox.getRadius();

	m_SceneNode->SetTranslate(-bbox.getCenter());
	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(radius * 1.5f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));

}

void CEditor3DPreviewScene::SetModel(IModelPtr Model)
{
	_ASSERT(Model != nullptr);

	Clean();

	auto modelComponent = m_ModelNode->GetComponentT<IModelsComponent3D>();
	if (modelComponent->GetModel())
		modelComponent->ResetModel();

	modelComponent->SetModel(Model);

	const auto& modelBBox = Model->GetBounds();
	float radius = modelBBox.getRadius();

	m_ModelNode->SetTranslate(- modelBBox.getCenter());
	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(radius * 1.5f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
}



//
// IGameState
//
void CEditor3DPreviewScene::Initialize()
{
	SceneBase::Initialize();

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light");
		lightNode->SetTranslate(glm::vec3(250.0f, 250.0f, 250.0f));
		lightNode->SetRotation(glm::vec3(-0.9f, -0.9f, -0.9f));

		lightNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponentT<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponentT<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponentT<ILightComponent3D>()->SetRange(1000.0f);
		lightNode->GetComponentT<ILightComponent3D>()->SetIntensity(1.2f);
		lightNode->GetComponentT<ILightComponent3D>()->SetSpotlightAngle(45.0f);
	}


	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
		GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}


	{
		m_SceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_SceneNode->SetName("SceneNodeProtoPreview");
	}

	{
		m_ModelNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_ModelNode->SetName("NodeModelPreview");
	}


	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("GridNodeX10");
		node->SetTranslate(glm::vec3(0.0f, 0.03f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponentT<IModelsComponent3D>()->SetModel(model);
	}



	//
	// Renderer
	//
	auto forwardrender = MakeShared(CRendererForward, GetBaseManager(), *this);
	forwardrender->Initialize(GetRenderWindow().GetRenderTarget());
	SetRenderer(forwardrender);
}

void CEditor3DPreviewScene::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}



//
// Private
//
void CEditor3DPreviewScene::Clean()
{
	if (m_SceneNode != nullptr)
	{
		while (m_SceneNode->GetChilds().size() > 0)
		{
			auto editorChild = *(m_SceneNode->GetChilds().begin());
			m_SceneNode->RemoveChild(editorChild);
		}
	}

	if (m_ModelNode != nullptr)
	{
		auto modelComponent = m_ModelNode->GetComponentT<IModelsComponent3D>();
		if (modelComponent->GetModel())
			modelComponent->ResetModel();
	}
}
