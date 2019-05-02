#pragma once

template<class T, typename... Args>
inline std::shared_ptr<T> Scene3D::CreateSceneNode(std::weak_ptr<SceneNode3D> Parent, Args &&... _Args)
{
    std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);
    newNode->SetScene(shared_from_this());
    newNode->SetParent(Parent);
    newNode->RegisterComponents();

    return newNode;
}