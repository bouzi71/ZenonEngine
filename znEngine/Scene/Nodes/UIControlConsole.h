#pragma once

#include "../../Materials/UI_Font_Material.h"

class ZN_API CUIControlConsole
	: public CUIControl
{
public:
	CUIControlConsole(IScene& Scene);
	virtual ~CUIControlConsole();

	void                                            Initialize() override;

	// CUITextNode
	std::shared_ptr<IznFont>                        GetFont() const;
	const std::shared_ptr<UI_Font_Material>&        GetMaterial() const;
	glm::vec2                                       GetTextSize() const;

	const std::string                               GetText() const;
	const glm::vec2                                 GetOffset() const;

	// CUIControl
	virtual glm::vec2                               GetSize() const override;

private:
	std::shared_ptr<IznFont>                        m_Font;
	std::shared_ptr<UI_Font_Material>               m_Material;

	std::shared_ptr<IPropertyT<std::string>> m_TextProperty;
	std::shared_ptr<IPropertyT<glm::vec2>>   m_OffsetProperty;
};