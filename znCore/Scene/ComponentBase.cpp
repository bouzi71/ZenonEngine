#include "stdafx.h"

// General
#include "ComponentBase.h"

CComponentBase::CComponentBase(std::shared_ptr<ISceneNode> OwnerNode)
    : m_OwnerNode(OwnerNode)
{
}

CComponentBase::~CComponentBase()
{
}

std::shared_ptr<ISceneNode> CComponentBase::GetOwnerNode() const
{
    return m_OwnerNode.lock();
}



bool CComponentBase::Accept(IVisitor* visitor)
{
    return false;
}


//
// ISceneNodeComponent
//
void CComponentBase::OnParentChanged()
{
    // do nothing
}

void CComponentBase::OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message)
{
    // do nothing
}



//
// Protected
//
void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
    std::shared_ptr<ISceneNode> ownerNode = m_OwnerNode.lock();
    _ASSERT(ownerNode != nullptr);

    ownerNode->RaiseComponentMessage(shared_from_this(), Message);
}