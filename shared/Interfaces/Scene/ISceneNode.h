#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IPropertiesG;
ZN_INTERFACE IScene;
ZN_INTERFACE ICameraComponent3D;
class Viewport;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IManager;
// FORWARD END



ZN_INTERFACE ZN_API __declspec(novtable) ISceneNode3D
	: public std::enable_shared_from_this<ISceneNode3D>
{
	typedef std::vector<std::shared_ptr<ISceneNode3D>>                Node3DList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNode3D>> Node3DNameMap;

	virtual ~ISceneNode3D() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	virtual void SetType(std::string Type) = 0;
	virtual std::string GetType() const = 0;
	virtual void SetName(std::string Name) = 0;
	virtual std::string	GetName() const = 0;

	virtual void AddChild(std::shared_ptr<ISceneNode3D> childNode) = 0;
	virtual void RemoveChild(std::shared_ptr<ISceneNode3D> childNode) = 0;
	virtual void SetParent(ISceneNode3D* parentNode) = 0;
	virtual ISceneNode3D* GetParent() const = 0;
	virtual const Node3DList& GetChilds() = 0;
	virtual void RaiseOnParentChanged() = 0;

	template<typename T, typename... Args> inline T* CreateSceneNode(Args &&... _Args)
	{
		return GetScene()->CreateSceneNode<T>(this, std::forward<Args>(_Args)...);
	}

	virtual IActionsGroup* GetActions() const = 0;
	virtual IPropertiesGroup* GetProperties() const = 0;
	virtual IScene* GetScene() const = 0;



	//
	// Transform functional
	//
	virtual void SetTranslate(cvec3 Translate) = 0;
	virtual void AddTranslate(vec3 Translate) = 0;
	virtual cvec3 GetTranslation() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetRotationQuaternion(cquat _rotate) = 0;
	virtual cquat GetRotationQuaternion() const = 0;

	virtual void SetScale(cvec3 _scale) = 0;
	virtual cvec3 GetScale() const = 0;

	virtual mat4 GetLocalTransform() const = 0;
	virtual mat4 GetInverseLocalTransform() const = 0;
	virtual void SetLocalTransform(cmat4 localTransform) = 0;

	virtual mat4 GetWorldTransfom() const = 0;
	virtual mat4 GetInverseWorldTransform() const = 0;
	virtual mat4 GetParentWorldTransform() const = 0;
	virtual void SetWorldTransform(cmat4 worldTransform) = 0;

	virtual void ForceRecalculateLocalTransform() = 0;



	//
	// Components engine
	//
	virtual bool IsComponentExists(GUID ComponentID) = 0;
	virtual ISceneNodeComponent* GetComponent(GUID ComponentID) = 0;
	virtual ISceneNodeComponent* AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) = 0;
	virtual const ComponentsMap& GetComponents() const = 0;
	virtual void RaiseComponentMessage(ISceneNodeComponent* Component, ComponentMessageType Message) = 0;
	virtual void RegisterComponents() = 0;

	template<typename T> inline bool IsComponentExists()
	{
		return IsComponentExists(__uuidof(T));
	}
	template<typename T> inline T* GetComponent()
	{
		if (ISceneNodeComponent* component = GetComponent(__uuidof(T)))
			return dynamic_cast<T*>(component);
		return nullptr;
	}
	template<typename T> inline T* AddComponent(std::shared_ptr<T> Component)
	{
		if (ISceneNodeComponent* component = AddComponent(__uuidof(T), Component))
			return dynamic_cast<T*>(component);
		return nullptr;
	}


	// Called before all others calls
	virtual void OnUpdate(UpdateEventArgs& e) = 0;
	virtual void DoUpdate(UpdateEventArgs& e) = 0;
	virtual void UpdateCamera(const ICameraComponent3D* camera) = 0;
	virtual void UpdateViewport(const Viewport* viewport) = 0;

	// Load & Save
	virtual bool Load(std::shared_ptr<IXMLReader> Reader) = 0;
	virtual bool Save(std::shared_ptr<IXMLWriter> Writer) = 0;

	// Visitor functionality
	virtual bool Accept(IVisitor* visitor) = 0;
};



ZN_INTERFACE ZN_API __declspec(novtable) ISceneNodeUI
	: public std::enable_shared_from_this<ISceneNodeUI>
{
	typedef std::vector<std::shared_ptr<ISceneNodeUI>>                NodeUIList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNodeUI>> NodeUINameMap;

	virtual ~ISceneNodeUI() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	// Name & Type
	virtual void SetType(std::string Type) = 0;
	virtual std::string GetType() const = 0;
	virtual void SetName(std::string Name) = 0;
	virtual std::string	GetName() const = 0;
	
	// Childs functional
	virtual void AddChild(std::shared_ptr<ISceneNodeUI> childNode) = 0;
	virtual void RemoveChild(std::shared_ptr<ISceneNodeUI> childNode) = 0;
	virtual void SetParent(ISceneNodeUI* parentNode) = 0;
	virtual ISceneNodeUI* GetParent() const = 0;
	virtual const NodeUIList& GetChilds() = 0;

	template<typename T, typename... Args> inline T* CreateSceneNode(Args &&... _Args)
	{
		return GetScene()->CreateSceneNodeUI<T>(this, std::forward<Args>(_Args)...);
	}

	// Actions & Properties
	virtual IActionsGroup* GetActions() const = 0;
	virtual IPropertiesGroup* GetProperties() const = 0;
	virtual IScene* GetScene() const = 0;

	//
	// Transform functional
	//
	virtual void SetTranslate(cvec2 _translate) = 0;
	virtual cvec2 GetTranslation() const = 0;
	virtual glm::vec2 GetTranslationAbs() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetScale(cvec2 _scale) = 0;
	virtual cvec2 GetScale() const = 0;
	virtual glm::vec2 GetScaleAbs() const = 0;
};



//
// For plugins
//
ZN_INTERFACE ZN_API __declspec(novtable) ISceneNodeCreator
{
	virtual ~ISceneNodeCreator() {}

	virtual size_t                                  GetSceneNodesCount() const = 0;
	virtual std::string                             GetSceneNodeTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<ISceneNode3D>             CreateSceneNode(std::weak_ptr<ISceneNode3D> Parent, size_t Index) const = 0;
};

ZN_INTERFACE ZN_API __declspec(novtable, uuid("9C3ACF8D-F30D-47AE-BBA1-D71DEA6B14D4")) ISceneNodesFactory
	: public IManager
{
	virtual ~ISceneNodesFactory() {}

	virtual void AddSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;
	virtual void RemoveSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;

	virtual std::shared_ptr<ISceneNode3D> CreateSceneNode(std::weak_ptr<ISceneNode3D> Parent, std::string SceneNodeTypeName) const = 0;
};
