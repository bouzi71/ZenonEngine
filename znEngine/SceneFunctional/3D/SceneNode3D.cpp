#include "stdafx.h"

// General
#include "SceneNode3D.h"

// Additional
#include "Loader.h"

SceneNode3D::SceneNode3D()
	: m_IsLoadingBegin(false)
	, m_IsLoaded(false)
{
	
}

SceneNode3D::~SceneNode3D()
{
}


std::shared_ptr<SceneNode3D> SceneNode3D::shared_from_this()
{
    return std::dynamic_pointer_cast<SceneNode3D>(base::shared_from_this());
}

std::weak_ptr<SceneNode3D> SceneNode3D::weak_from_this()
{
    return std::weak_ptr<SceneNode3D>(shared_from_this());
}



//
// Components engine
//
void SceneNode3D::RegisterComponents()
{
    SetTransformComponent(AddComponent(std::make_shared<CTransformComponent3D>(shared_from_this())));
    SetMeshComponent(AddComponent(std::make_shared<CMeshComponent3D>(shared_from_this())));
    SetColliderComponent(AddComponent(std::make_shared<CColliderComponent3D>(shared_from_this())));

    AddComponent(std::make_shared<CLightComponent3D>(shared_from_this()));
}


void SceneNode3D::UpdateCamera(const ICamera* camera)
{
	// Do nothing...
}

bool SceneNode3D::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->Visit(this);
    if (visitResult)
    {
		const auto& components = GetComponents();
		std::for_each(components.begin(), components.end(), [&visitor](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
		{
			Component.second->Accept(visitor);
		});
    }

	// Now visit children
	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode>& Child)
	{
#ifdef LOADER_ENABLED
		std::shared_ptr<ILoadable> loadable = std::dynamic_pointer_cast<ILoadable, SceneNode3D>(child);
		if (loadable != nullptr && !loadable->isLoaded())
			continue;
#endif

		Child->Accept(visitor);
	});

	return visitResult;
}

void SceneNode3D::OnUpdate(UpdateEventArgs & e)
{
}



//
// // ILoadableObject
//
bool SceneNode3D::PreLoad()
{
	return false;
}

bool SceneNode3D::Load()
{
	return false;
}

bool SceneNode3D::Delete()
{
	return false;
}

void SceneNode3D::setLoadingBegin()
{
	m_IsLoadingBegin = true;
}

bool SceneNode3D::isLoadingBegin() const
{
	return m_IsLoadingBegin;
}

void SceneNode3D::setLoaded()
{
	m_IsLoaded = true;
}

bool SceneNode3D::isLoaded() const
{
	return m_IsLoaded;
}

uint32 SceneNode3D::getPriority() const
{
	return 0;
}



//
// Protected
//
void SceneNode3D::SetTransformComponent(std::shared_ptr<CTransformComponent3D> TransformComponent)
{
    m_Components_Transform = TransformComponent;
}

void SceneNode3D::SetMeshComponent(std::shared_ptr<CMeshComponent3D> MeshComponent)
{
	m_Components_Mesh = MeshComponent;
}

void SceneNode3D::SetColliderComponent(std::shared_ptr<CColliderComponent3D> ColliderComponent)
{
    m_Components_Collider = ColliderComponent;
}
