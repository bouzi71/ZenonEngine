#include "stdafx.h"

// General
#include "UIBeizerLine.h"

CUIBeizerLineNode::CUIBeizerLineNode(IRenderDevice* RenderDevice)
{
	m_Material = std::make_shared<UI_Line_Material>(RenderDevice);
	m_Material->SetWrapper(m_Material.get());;

    m_StartPoint = vec2(0.0f, 0.0f);
    m_EndPoint = vec2(100.0f, 100.0f);

    vec2 p[4];
    p[0] = vec2(0.0f, 0.0f);
    p[1] = vec2(50.0f, 0.0f);
    p[2] = vec2(50.0f, 100.0f);
    p[3] = vec2(100.0f, 100.0f);

    m_Mesh = RenderDevice->CreateMesh();
    m_Mesh->SetPrimitiveTopology(PrimitiveTopology::LineList);

    m_PointsBuffer = RenderDevice->CreateVertexBuffer(p, 4);
    m_Mesh->AddVertexBuffer(BufferBinding("POSITION", 0), m_PointsBuffer);
	m_Mesh->SetMaterial(m_Material);
}

CUIBeizerLineNode::~CUIBeizerLineNode()
{}



//
// CUIBeizerLineNode
//
void CUIBeizerLineNode::SetColor(vec4 Color)
{
	m_Material->SetColor(Color);
}

void CUIBeizerLineNode::SetPosStart(vec2 PosStart)
{
    m_StartPoint = PosStart;
    UpdateBuffer();
}

void CUIBeizerLineNode::SetPosEnd(vec2 PosEnd)
{
    m_EndPoint = PosEnd;
    UpdateBuffer();
}

void CUIBeizerLineNode::SetThickness(float Thickness)
{
    m_Material->SetThickness(Thickness);
}



//
// CUIBaseNode
//
glm::vec2 CUIBeizerLineNode::GetSize()
{
    return glm::vec2(9999.0f, 9999.0f);
}

bool CUIBeizerLineNode::AcceptMesh(IVisitor* visitor)
{
	return m_Mesh->Accept(visitor);
}

void CUIBeizerLineNode::UpdateBuffer()
{
    float middle = m_StartPoint.x + (m_EndPoint - m_StartPoint).x / 2.0f;

    vec2 P1 = m_StartPoint;
    P1.x = middle;

    vec2 P2 = m_EndPoint;
    P2.x = middle;

    vec2 p[4];
    p[0] = m_StartPoint;
    p[1] = P1;
    p[2] = P2;
    p[3] = m_EndPoint;

    IBuffer* pointsBufferNew = GetBaseManager()->GetManager<IRenderDevice>()->CreateVertexBuffer(p, 4);
    m_PointsBuffer->Copy(pointsBufferNew);
	GetBaseManager()->GetManager<IRenderDevice>()->DestroyVertexBuffer(pointsBufferNew);
}
