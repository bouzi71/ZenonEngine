#pragma once

#include "../../Materials/UI_Line_Material.h"

class ZN_API CUIBeizerLineNode 
	: public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUIBeizerLineNode(IRenderDevice& RenderDevice);
	virtual	~CUIBeizerLineNode();

	// CUIBeizerLineNode
	void                                            SetColor(vec4 Color);
    void                                            SetPosStart(vec2 PosStart);
    void                                            SetPosEnd(vec2 PosEnd);
    void                                            SetThickness(float Thickness);

	// CUIBaseNode
    virtual glm::vec2                               GetSize() override final;

	virtual bool                                    AcceptMesh(IVisitor* visitor) override;

protected:
    void UpdateBuffer();

private:
	std::shared_ptr<IMesh>              m_Mesh;
    std::shared_ptr<IBuffer>            m_PointsBuffer;
	std::shared_ptr<UI_Line_Material>   m_Material;

    vec2                                m_StartPoint;
    vec2                                m_EndPoint;
};
