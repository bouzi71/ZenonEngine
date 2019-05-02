#pragma once

#include "SceneFunctional/SceneNode/TransformComponent.h"
#include "SceneFunctional/SceneNode/MeshComponent.h"
#include "SceneFunctional/SceneNode/ColliderComponent.h"
#include "SceneFunctional/SceneNode/LightComponent.h"

// FORWARD BEGIN
class Scene3D;
// FORWARD END

class SceneNode3D : public Object, public ILoadable, public virtual std::enable_shared_from_this<SceneNode3D>
{
	typedef Object base;
    friend Scene3D;
public:
    typedef std::map<GUID, std::shared_ptr<ISceneNodeComponent>>        ComponentsMap;
    //typedef std::function<void(std::shared_ptr<ISceneNodeComponent>)>   ComponentsMessageType;
    //typedef std::map<uint32, ComponentsMessageType>                     ComponentsMessagesMap;
	typedef std::vector<std::shared_ptr<SceneNode3D>>                   NodeList;
	typedef std::multimap<std::string, std::shared_ptr<SceneNode3D>>    NodeNameMap;

public:
                                                    SceneNode3D();
	virtual                                         ~SceneNode3D();

	const std::string&                              GetName() const;
	void                                            SetName(const std::string& name);

    // Components engine
    bool                                            IsComponentExists(GUID ComponentID);
    std::shared_ptr<ISceneNodeComponent>            GetComponent(GUID ComponentID);
    std::shared_ptr<ISceneNodeComponent>            AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component);
    void                                            RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message);
    virtual void                                    RegisterComponents();

    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              IsComponentExists();
    template<typename T>
    std::shared_ptr<T>                              GetComponent();
    template<typename T>
    std::shared_ptr<T>                              AddComponent(std::shared_ptr<T> Component);

    // Scene access
    void                                            SetScene(std::shared_ptr<Scene3D> Scene);
    std::shared_ptr<Scene3D>                        GetScene() const;

    template<typename T, typename... Args>
    std::shared_ptr<T>                              CreateSceneNode(Args &&... _Args);

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<SceneNode3D> childNode);
	virtual void                                    RemoveChild(std::shared_ptr<SceneNode3D> childNode);
	virtual void                                    SetParent(std::weak_ptr<SceneNode3D> parentNode);
	virtual std::shared_ptr<SceneNode3D>            GetParent() const;
	virtual NodeList                                GetChilds();

	// Called before all others calls
	virtual void                                    UpdateCamera(const Camera* camera);

	// Allow a visitor to visit this node.
	virtual bool                                    Accept(IVisitor& visitor);

	// Updatable
	virtual void                                    OnUpdate(UpdateEventArgs& e);

	// ILoadableObject
	virtual bool                                    PreLoad()             override;
	virtual bool                                    Load()                override;
	virtual bool                                    Delete()              override;
	void                                            setLoadingBegin()   override final;
	bool                                            isLoadingBegin()    const override final;
	void                                            setLoaded()         override final;
	bool                                            isLoaded()              const override final;
	virtual uint32                                  getPriority() const override;

protected:
    void                                            SetTransformComponent(std::shared_ptr<CTransformComponent> TransformComponent);
    void                                            SetColliderComponent(std::shared_ptr<CColliderComponent> ColliderComponent);

    // Callbacks caller
    void                                            RaiseOnParentChanged();

private:
	std::string                                     m_Name;
    ComponentsMap                                   m_Components;
    //ComponentsMessagesMap                           m_ComponentsMessages;
    std::shared_ptr<CTransformComponent>            m_Components_Transform;
    std::shared_ptr<CColliderComponent>             m_Components_Collider;

    std::weak_ptr<Scene3D>                          m_Scene;
	std::weak_ptr<SceneNode3D>                      m_ParentNode;
	NodeList                                        m_Children;
	NodeNameMap                                     m_ChildrenByName;
	std::mutex                                      m_ChildMutex;

private:
	std::atomic<bool>                               m_IsLoadingBegin;
	std::atomic<bool>                               m_IsLoaded;
};

#include "SceneNode3D.inl"
