#pragma once

//
// Components engine template access
//
template<typename T>
inline std::shared_ptr<T> CUIBaseNode::GetComponent()
{
    return base::GetComponent<T>();
}



//
// Components engine template access specialization
//
template<>
inline std::shared_ptr<CTransformComponentUI> CUIBaseNode::GetComponent<CTransformComponentUI>()
{
    return m_Components_Transform;
}
