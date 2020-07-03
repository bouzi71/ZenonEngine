#include "stdafx.h"

// General
#include "SceneNodeTreeItem.h"

CSceneNodeTreeItem::CSceneNodeTreeItem()
	: m_SceneNode3D(nullptr)
	, m_Parent(nullptr)
{
}

CSceneNodeTreeItem::CSceneNodeTreeItem(const std::shared_ptr<ISceneNode3D>& SceneNode3D, CSceneNodeTreeItem * Parent)
	: m_SceneNode3D(SceneNode3D)
	, m_Parent(Parent)
{
	for (const auto& ch : SceneNode3D->GetChilds())
	{
		CSceneNodeTreeItem* treeItem = new CSceneNodeTreeItem(ch, this);
		addChild(treeItem);
	}
}

CSceneNodeTreeItem::~CSceneNodeTreeItem() 
{ 
	qDeleteAll(m_Childs); 
}


void CSceneNodeTreeItem::addChild(CSceneNodeTreeItem * item) 
{
	m_Childs.push_back(item);
}

CSceneNodeTreeItem* CSceneNodeTreeItem::child(int row) 
{
	if (row >= m_Childs.size())
		return nullptr;

	return m_Childs.at(row);
}

int CSceneNodeTreeItem::childCount() const 
{
	return m_Childs.size();
}

CSceneNodeTreeItem* CSceneNodeTreeItem::parentItem()
{
	return m_Parent;
}



QVariant CSceneNodeTreeItem::data() const
{
	if (m_SceneNode3D == nullptr)
		return QVariant("Real root element");

	return QVariant(m_SceneNode3D->GetName().c_str());
}



int CSceneNodeTreeItem::childNumberInParent() const
{
	if (m_Parent)
	{
		const auto& it = std::find(m_Parent->m_Childs.begin(), m_Parent->m_Childs.end(), this);
		if (it != m_Parent->m_Childs.end())
			return std::distance(m_Parent->m_Childs.begin(), it);
	}

	return 0;
}

std::shared_ptr<ISceneNode3D> CSceneNodeTreeItem::GetSceneNode() const
{
	return m_SceneNode3D;
}