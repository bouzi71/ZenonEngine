#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
ZN_INTERFACE IGeometry;
class RenderEventArgs;
// FORWARD END

/**
  * ������ ������������ ����� ������������ ��������� ������������ ��������� (��������: ��������� ����� ����� � ��������� ����������),
  * ���������, ������������ ��� ��������� ������ ��������� (��������: ������ ��� ����� � ������ ��� ����������)
  * � ����� ��������� � ��������� ����� �����
  *
*/
ZN_INTERFACE ZN_API __declspec(novtable) IModel 
	: public std::enable_shared_from_this<IModel>
{
	virtual ~IModel() {}

	virtual void                                    SetName(const std::string& Name) = 0;
	virtual std::string                             GetName() const = 0;

	virtual void                                    SetBounds(const BoundingBox& Bounds) = 0;
	virtual BoundingBox                             GetBounds() const = 0;

	/**
	 * ��������� ���� ���������[+GeometryPart] - ��������
	*/
	virtual void									AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;

	virtual bool                                    Render(const RenderEventArgs& renderEventArgs) const = 0;

	// For IVisitor

	virtual bool                                    Accept(IVisitor* visitor) = 0;
};
typedef std::shared_ptr<IModel> IModelPtr;

typedef std::vector<std::shared_ptr<IModel>> MeshList;
