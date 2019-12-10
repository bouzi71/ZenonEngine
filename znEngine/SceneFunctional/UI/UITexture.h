#pragma once

#include "../../Materials/UI_Texture_Material.h"

class OW_ENGINE_API CUITextureNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUITextureNode(std::shared_ptr<IRenderDevice> RenderDevice, vec2 Size = vec2(1.0f, 1.0f));
	virtual ~CUITextureNode();

	// CUITextureNode
	void SetTexture(std::shared_ptr<ITexture> _texture);
	void SetColor(vec4 _color);

	// CUIBaseNode
    virtual glm::vec2 GetSize() override;

	virtual bool AcceptMesh(IVisitor* visitor) override;

private:
    glm::vec2                               m_Size;
	std::shared_ptr<IMesh>                  m_Mesh;
	std::shared_ptr<UI_Texture_Material>    m_Material;
};