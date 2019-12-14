#pragma once

class OW_ENGINE_API CComponentBase 
	: public ISceneNodeComponent
{
public:
    CComponentBase(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CComponentBase();

    std::shared_ptr<ISceneNode>                     GetOwnerNode() const;

    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              IsComponentExists();
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

    // Accept from SceneNode
    virtual bool                                    Accept(IVisitor* visitor) override;

    // ISceneNodeComponent
    virtual void                                    OnParentChanged() override;
    virtual void                                    OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) override;

protected:
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    std::weak_ptr<ISceneNode>                       m_OwnerNode;
};

#include "ComponentBase.inl"