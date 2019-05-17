#pragma once

#ifndef ONLY_SIMPLE_UI

// Common UI
#include "SceneFunctional/UI/SceneNodeUI.h"
#include "UIColor.h"
#include "UIText.h"

// ContainerGarmoshka

// FORWARD BEGIN
class CUIContainerGarmoshka;
// FORWARD END

class CUIContainerGarmoshkaCategoryHeader : public CUIBaseNode
{
public:
    CUIContainerGarmoshkaCategoryHeader(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka);
    virtual ~CUIContainerGarmoshkaCategoryHeader();

    // CUIContainerGarmoshkaCategoryHeader
    void Initialize();
    void SetText(const std::string& Text);

    // CUIBaseNode
    glm::vec2 GetSize() const override final;
    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override final;

    // Input events
    virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override final;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override final;

private:
    std::shared_ptr<CUIColorNode>  m_Background;
    std::shared_ptr<CUITextNode>   m_Text;

    // Weak refs
    std::weak_ptr<CUIContainerGarmoshka>                    m_ContainerGarmoshka;
};

#endif
