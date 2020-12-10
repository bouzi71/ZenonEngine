#pragma once

#include "Interfaces/Render/Types/RenderTypes.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IUIControl;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE ILight;
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
  *   1) ���������� �������� Accept(IVisitor*) � ISceneNode
  *   2) ISceneNode �������� ��� �� �������� (� ������ ���������� �������������) Visit(ISceneNode*) � ���������� � �������� � ���� ����
  *   3) ���������� �������� ISceneNode ��� "�����" ������ ���� ������� ���� ���
  *   4 �����) � ������ ���� ����� ���� ��� ������� �����������, �� ����    
*/
ZN_INTERFACE ZN_API IVisitor 
{
	virtual ~IVisitor() {}

	virtual EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& SceneNode) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNodeComponent>& Component) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<ILight>& Light) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem) = 0;
};