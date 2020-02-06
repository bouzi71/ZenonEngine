#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"

// FORWARD BEGIN
ZN_INTERFACE IBuffer;
ZN_INTERFACE IShader;
class RenderEventArgs;
// FORWARD END


/**
  * ��������� �������� � ���� ����������:
  *   - ��������� ������� ������� (�������� ������ �������, ������ �������� � ������ ���������� ���������),
  *   - ������ ����������� ������ � ������� ������ - PrimitiveTopology
  *   - BoundingBox ��� �������� ������ ������
  * 
  * ! ����� �������, ��� ��� ���������� ��������� ������� ��������� �������������� ��������� �������� �
  *   ���������, ����������� ��� ������� ������� (World �������). ������� ������� ��������� ������ ������ � ��������� ������, 
  *   ��� �������� � ���� ������ � ������ �������.
*/
ZN_INTERFACE ZN_API IGeometry : public std::enable_shared_from_this<IGeometry>
{
	typedef std::unordered_map<BufferBinding, std::shared_ptr<IBuffer>> BuffersMap;

	virtual ~IGeometry() {}

	virtual void SetBounds(const BoundingBox& Bounds) = 0;
	virtual BoundingBox GetBounds() const = 0;

	/** �������� ������ � ���������� ������� ������������ ���� (POSITION, TEXCOORD, NORMAL � ��)
	  * ������������ �������� ������ ������ ������� (POSITION). ��������� ������� �������� �������������.
	  *
	  * ! ����� ������� ��� �� BufferBinding (��� � ����) ������� � InputSemantic ���������� �������.
	  */
	virtual void AddVertexBuffer(const BufferBinding& Binding, const std::shared_ptr<IBuffer>& VertexBuffer) = 0;

	/** ���������� ������ � ������ ������� ��������� ������. 
	  * ������ � ������� ������������� ������ � �� ������ ��������� ��������� � InputSemantics ��� ����������� ���������� �������.
	  */
	virtual void SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer) = 0;
	virtual void SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology Topology) = 0;

	virtual bool Render(         const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) const = 0;
	virtual bool RenderInstanced(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs = SGeometryDrawInstancedArgs()) const = 0;
	
	// For IVisitor

	virtual void Accept(IVisitor* visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
};
