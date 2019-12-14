#pragma once

#include "../SceneNodeBase.h"

// FORWARD BEGIN
class CUIBaseNode;
class CUIWindowNode;
// FORWARD END


class OW_ENGINE_API UIBaseNodeClickedEventArgs
{
public:
    UIBaseNodeClickedEventArgs(std::shared_ptr<CUIBaseNode> Initiator)
        : Initiator(Initiator)
    {}

    std::shared_ptr<CUIBaseNode> Initiator;
};
typedef Delegate<UIBaseNodeClickedEventArgs> UIBaseNodeClickedEvent;


class OW_ENGINE_API CUIBaseNode 
	: public SceneNodeBase
	, public ISceneNodeUI
{
	friend IScene;
public:
	explicit CUIBaseNode();
	virtual ~CUIBaseNode();

    std::shared_ptr<CUIBaseNode>                    shared_from_this();
    std::weak_ptr<CUIBaseNode>                      weak_from_this();

    // Components engine
    virtual void                                    RegisterComponents() override;
    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

	// Size & bounds functional
    virtual glm::vec2                               GetSize();
    virtual BoundingRect                            GetBoundsAbs();
    virtual bool                                    IsPointInBoundsAbs(glm::vec2 Point) ;

	// Allow a visitor to visit this node. 
	virtual bool                                    Accept(IVisitor* visitor) override;
	virtual bool                                    AcceptMesh(IVisitor* visitor);

	// Input events
	virtual bool                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e);

    UIBaseNodeClickedEvent Clicked;

	// Syntetic events
	virtual void                                    OnMouseEntered();
	virtual void                                    OnMouseLeaved();

public: // Syntetic events // TODO: Make private
	bool                                            IsMouseOnNode() const;
	void                                            DoMouseEntered();
	void                                            DoMouseLeaved();

protected:
    void                                            SetTransformComponent(std::shared_ptr<ITransformComponentUI> TransformComponent);

private:
    std::shared_ptr<ITransformComponentUI>          m_Components_Transform;

private: // Syntetic events
	bool                                            m_IsMouseOnNode;
};

#include "SceneNodeUI.inl"