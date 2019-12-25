#pragma once

#if 0

class ZN_API CSceneNodeProxie
	: public ISceneNode
	, public ISceneNodeWrapper
{
public:
	CSceneNodeProxie();
	virtual ~CSceneNodeProxie();


	//
	// ISceneNode
	//
	virtual void									Initialize();
	virtual void									Finalize();

	virtual std::string								GetName() const;
	virtual void                                    SetName(const std::string& name);

	// Components engine
	virtual bool                                    IsComponentExists(GUID ComponentID);
	virtual std::shared_ptr<ISceneNodeComponent>    GetComponent(GUID ComponentID);
	virtual std::shared_ptr<ISceneNodeComponent>    AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component);
	virtual const ComponentsMap&                    GetComponents() const;
	virtual void                                    RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message);
	virtual void                                    RegisterComponents();

	// Scene access
	virtual void                                    SetScene(std::shared_ptr<IScene> Scene);
	virtual std::shared_ptr<IScene>                 GetScene() const;

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode> childNode);
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode> childNode);
	virtual void                                    SetParent(std::weak_ptr<ISceneNode> parentNode);
	virtual std::shared_ptr<ISceneNode>             GetParent() const;
	virtual std::vector<std::shared_ptr<ISceneNode>>GetChilds();

	// Called before all others calls
	virtual void                                    UpdateCamera(const ICamera* camera);
	virtual void                                    UpdateViewport(const Viewport* viewport);

	virtual bool                                    Accept(IVisitor* visitor);

	virtual void                                    OnUpdate(UpdateEventArgs& e);

	std::shared_ptr<ISettingGroup>					GetProperties() const override;
	
	//
	// ISceneNodeWrapper
	//
	void											SetWrappedNode(std::shared_ptr<ISceneNode> ThisNode) override final;
	std::shared_ptr<ISceneNode>                     GetWrappedNode() const override final;

	void                                            RaiseOnParentChanged() override;


	//
	// Templates
	//
	template<typename T>
	inline std::shared_ptr<T> IsComponentExists()
	{
		return ISceneNode::IsComponentExists<T>();
	}
	template<typename T>
	inline std::shared_ptr<T> GetComponent()
	{
		return ISceneNode::GetComponent<T>();
	}
	template<typename T>
	inline std::shared_ptr<T> AddComponent(std::shared_ptr<T> Component)
	{
		return ISceneNode::AddComponent<T>(Component);
	}

	template<typename T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNode(Args &&... _Args)
	{
		return ISceneNode::CreateSceneNode<T>(std::forward<Args>(_Args)...);
	}

protected:
	IBaseManager* GetBaseManager() const;

private:
	std::shared_ptr<ISceneNode> m_SceneNode;
};

#endif
