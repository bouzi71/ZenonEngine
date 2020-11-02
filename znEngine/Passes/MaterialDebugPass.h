#pragma once

class CMaterial_Debug_Pass 
	: public Base3DPass
{
public:
	CMaterial_Debug_Pass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CMaterial_Debug_Pass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override final;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
};