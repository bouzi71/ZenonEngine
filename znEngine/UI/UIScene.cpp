#include "stdafx.h"


// General
#include "UIScene.h"

CUIScene::CUIScene()
{
	m_RootNode = std::make_shared<CUIWindowNode>();
}

CUIScene::~CUIScene()
{
}

std::shared_ptr<CUIWindowNode> CUIScene::GetRootNode() const
{
	return m_RootNode;
}

void CUIScene::Accept(IVisitor & visitor)
{
	if (m_RootNode)
		m_RootNode->Accept(visitor);
}


//
// Input events
//
bool CUIScene::OnKeyPressed(KeyEventArgs & e)
{
	if (m_RootNode)
		return DoKeyPressed_Rec(m_RootNode, e);

	return false;
}

void CUIScene::OnKeyReleased(KeyEventArgs & e)
{
	if (m_RootNode)
        DoKeyReleased_Rec(m_RootNode, e);
}

void CUIScene::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_RootNode)
        DoMouseMoved_Rec(m_RootNode, e);
}

bool CUIScene::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_RootNode)
		return DoMouseButtonPressed_Rec(m_RootNode, e);

	return false;
}

void CUIScene::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_RootNode)
        DoMouseButtonReleased_Rec(m_RootNode, e);
}

bool CUIScene::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_RootNode)
		return DoMouseWheel_Rec(m_RootNode, e);

	return false;
}



//
// Input events process recursive
//
bool CUIScene::DoKeyPressed_Rec(const std::shared_ptr<CUIBaseNode>& Node, KeyEventArgs & e)
{
    for (auto child : Node->GetChilds())
    {
        if (DoKeyPressed_Rec(child, e))
            return true;
    }

    if (Node->OnKeyPressed(e))
        return true;

    return false;
}

void CUIScene::DoKeyReleased_Rec(const std::shared_ptr<CUIBaseNode>& Node, KeyEventArgs & e)
{
    for (auto child : Node->GetChilds())
    {
        DoKeyReleased_Rec(child, e);
    }

    Node->OnKeyReleased(e);
}

void CUIScene::DoMouseMoved_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseMotionEventArgs & e)
{
    for (auto child : Node->GetChilds())
    {
        DoMouseMoved_Rec(child, e);
    }

    Node->OnMouseMoved(e);

    // Synteric events impl
    if (Node->IsPointInBoundsAbs(e.GetPoint()))
    {
        if (!Node->IsMouseOnNode())
        {
            Node->OnMouseEntered();
            Node->DoMouseEntered();
        }
    }
    else
    {
        if (Node->IsMouseOnNode())
        {
            Node->OnMouseLeaved();
            Node->DoMouseLeaved();
        }
    }
}

bool CUIScene::DoMouseButtonPressed_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseButtonEventArgs & e)
{
    for (auto child : Node->GetChilds())
    {
        if (DoMouseButtonPressed_Rec(child, e))
            return true;
    }

    if (Node->IsPointInBoundsAbs(e.GetPoint()))
        if (Node->OnMouseButtonPressed(e))
            return true;

    return false;
}

void CUIScene::DoMouseButtonReleased_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseButtonEventArgs & e)
{
    for (auto child : Node->GetChilds())
    {
        DoMouseButtonReleased_Rec(child, e);
    }

    Node->OnMouseButtonReleased(e);
}

bool CUIScene::DoMouseWheel_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseWheelEventArgs & e)
{
    for (auto child : Node->GetChilds())
    {
        if (DoMouseWheel_Rec(child, e))
            return true;
    }

    if (Node->IsPointInBoundsAbs(e.GetPoint()))
        if (Node->OnMouseWheel(e))
            return true;

    return false;
}
