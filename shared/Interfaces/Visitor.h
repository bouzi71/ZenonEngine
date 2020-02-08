#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"
#include "Interfaces/Render/Types/ShaderType.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ILightComponent3D;
// FORWARD END

/**
  * ������� ���������� ������������ � ZenonEngine � ��������� ��� ��������� �����.
  *
  * ������ ��� 3D:
  *   1) ���������� �������� Accept(IVisitor*) � ISceneNode3D
  *   2) ISceneNode3D �������� ��� �� �������� (� ������ ���������� �������������) Visit(ISceneNode3D*) � ���������� � �������� � ���� ����
  *   3) ���������� �������� ISceneNode3D ��� "�����" ������ ���� ������� ���� ���
  *   4 �����) � ������ ���� ����� ���� ��� ������� �����������, �� ����    
*/
ZN_INTERFACE ZN_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool Visit(const ISceneNode3D* node) = 0;
	virtual bool Visit(const ISceneNodeUI* node) = 0;
	virtual bool Visit(const IModel* Model) = 0;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual bool Visit(const ILightComponent3D* Light) = 0;
};