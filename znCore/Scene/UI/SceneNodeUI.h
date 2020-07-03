#pragma once

class ZN_API SceneNodeUI 
	: public ISceneNodeUI
{
	friend IScene;
public:
	SceneNodeUI();
	virtual ~SceneNodeUI();

	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;
	
	// Childs functional
	virtual void                                    AddChild(const std::shared_ptr<ISceneNodeUI>& childNode) override final;
	virtual void                                    RemoveChild(const std::shared_ptr<ISceneNodeUI>& childNode) override final;
	virtual std::weak_ptr<ISceneNodeUI>             GetParent() const override final;
	virtual const NodeUIList&                       GetChilds() override final;
	void                                            RaiseOnParentChanged() override final;

	// Actions & Properties
	virtual std::shared_ptr<IPropertiesGroup>       GetProperties() const final;
	virtual IScene*                                 GetScene() const final;

	// ISceneNodeUI
	void											SetTranslate(const glm::vec2& _translate);
	const glm::vec2&								GetTranslation() const;
	glm::vec2										GetTranslationAbs() const;
	void											SetRotation(const glm::vec3& _rotate);
	const glm::vec3&								GetRotation() const;
	void											SetScale(const glm::vec2& _scale);
	const glm::vec2&								GetScale() const;
	glm::vec2										GetScaleAbs() const;
	virtual glm::mat4								GetLocalTransform() const;
	virtual glm::mat4								GetWorldTransfom() const;

	// Size & bounds functional
    virtual glm::vec2                               GetSize() const override;
    virtual BoundingRect                            GetBoundsAbs() override;
    virtual bool                                    IsPointInBoundsAbs(const glm::vec2& Point) override;

	// Allow a visitor to visit this node. 
	virtual void                                    Accept(IVisitor* visitor);
	virtual void                                    AcceptMesh(IVisitor* visitor);

	// UI events
	void                                            SetOnClickCallback(std::function<void(const ISceneNodeUI* Node, glm::vec2)> OnClickCallback);

	// Input events
	virtual bool                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e);

	// Syntetic events
	virtual void                                    OnMouseEntered();
	virtual void                                    OnMouseLeaved();

private:
	void                                            SetSceneInternal(const std::weak_ptr<IScene>& Scene);
	void                                            AddChildInternal(const std::shared_ptr<ISceneNodeUI>& ChildNode);
	void                                            RemoveChildInternal(const std::shared_ptr<ISceneNodeUI>& ChildNode);
	void                                            SetParentInternal(const std::weak_ptr<ISceneNodeUI>& parentNode);

protected:
	virtual void									UpdateLocalTransform();
	virtual void									UpdateWorldTransform();
	IBaseManager&                                   GetBaseManager() const;

public: // Syntetic events // TODO: Make private
	bool                                            IsMouseOnNode() const;
	void                                            DoMouseEntered();
	void                                            DoMouseLeaved();
	bool                                            m_IsMouseOnNode;

private:
	NodeUIList                                      m_Children;
	NodeUINameMap                                   m_ChildrenByName;
	std::weak_ptr<ISceneNodeUI>                     m_ParentNode;

	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;
	std::weak_ptr<IScene>                           m_Scene;

private:
	glm::vec2										m_Translate;
	glm::vec3										m_Rotate;
	glm::vec2										m_Scale;
	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;

private:
	std::function<void(const ISceneNodeUI* Node, glm::vec2)> m_OnClickCallback;
};

