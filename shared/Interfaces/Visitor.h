#pragma once

#include "Interfaces/Render/Types/RenderTypes.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE ILight3D;
ZN_INTERFACE IParticleSystem;
// FORWARD END

enum ZN_API EVisitResult : size_t
{
	Block = 1 << 0,
	AllowVisitChilds = 1 << 1,
	AllowVisitContent = 1 << 2,
	AllowAll = AllowVisitChilds | AllowVisitContent
};

/**
  * ������� ���������� ������������ � ZenonEngine � ��������� ��� ��������� �����.
  *
  * ������ ��� 3D:
  *   1) ���������� �������� Accept(IVisitor*) � ISceneNode3D
  *   2) ISceneNode3D �������� ��� �� �������� (� ������ ���������� �������������) Visit(ISceneNode3D*) � ���������� � �������� � ���� ����
  *   3) ���������� �������� ISceneNode3D ��� "�����" ������ ���� ������� ���� ���
  *   4 �����) � ������ ���� ����� ���� ��� ������� �����������, �� ����    
*/
ZN_INTERFACE ZN_API IVisitor 
{
	virtual ~IVisitor() {}

	virtual EVisitResult Visit(const ISceneNode3D* SceneNode) = 0;
	virtual EVisitResult Visit(const ISceneNodeUI* SceneNode) = 0;
	virtual EVisitResult Visit(const IModel* Model) = 0;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual EVisitResult Visit(const ISceneNodeComponent* Component) = 0;
	virtual EVisitResult Visit(const ILight3D* Light) = 0;
	virtual EVisitResult Visit(const IParticleSystem* ParticleSystem) = 0;
};