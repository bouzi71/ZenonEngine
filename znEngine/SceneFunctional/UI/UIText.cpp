#include "stdafx.h"

// General
#include "UIText.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const vec2  cDefaultOffset = vec2(0.0f, 0.0f);
	const vec4  cDefaultColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUITextNode::CUITextNode()
{}

CUITextNode::~CUITextNode()
{}



void CUITextNode::Initialize()
{
	m_TextProperty = std::make_shared<CSettingBase<std::string>>(cDefaultText);
	GetProperties()->AddSetting("Text", m_TextProperty);

	m_OffsetProperty = std::make_shared<CSettingBase<glm::vec2>>(cDefaultOffset);
	GetProperties()->AddSetting("Offset", m_OffsetProperty);

	m_Font = GetManager<IFontsManager>(GetBaseManager())->GetMainFont();

	m_Material = std::make_shared<UI_Font_Material>(GetManager<IRenderDevice>(GetBaseManager()));
	m_Material->SetWrapper(m_Material);
	m_Material->SetTexture(0, m_Font->GetTexture());
	m_Material->SetColor(cDefaultColor);
}



//
// CUITextNode
//
void CUITextNode::SetFont(std::shared_ptr<CFontMesh> _font)
{
    _ASSERT(_font != nullptr);

    m_Font = _font;
    m_Material->SetTexture(0, m_Font->GetTexture());
}

std::shared_ptr<CFontMesh> CUITextNode::GetFont() const
{
    return m_Font;
}

void CUITextNode::SetTextColor(cvec4 _color)
{
    m_Material->SetColor(_color);
}

glm::vec2 CUITextNode::GetTextSize() const
{
    float width = m_Font->GetStringWidth(m_TextProperty->Get());
    float height = m_Font->GetHeight();

    return glm::vec2(width, height);
}



//
// CUIBaseNode
//
glm::vec2 CUITextNode::GetSize()
{
    return GetTextSize() + 2.0f * m_OffsetProperty->Get();
}

bool CUITextNode::AcceptMesh(IVisitor* visitor)
{
	m_Font->SetMaterial(m_Material);

	const std::string& _text = m_TextProperty->Get();
	vec2 _offset = m_OffsetProperty->Get();

	for (uint32 i = 0; i < _text.length(); i++)
	{
		uint8 ch = _text.c_str()[i];
		m_Material->SetOffset(_offset);
		_offset.x += static_cast<float>(m_Font->GetCharWidth(ch)) + 0.01f;

		m_Font->Accept(visitor, 0, 0, (ch) * 6, 6);
	}

	return true;
}
