#pragma once

class CDrawLightFrustumPass
	: public Base3DPass
{
public:
	CDrawLightFrustumPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CDrawLightFrustumPass();

	void SetNeedRefresh();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	EVisitResult Visit(const ISceneNode* node) override final;
	EVisitResult Visit(const ILight3D* Light) override final;

protected:
	bool                               m_IsDirty;

	std::shared_ptr<MaterialDebug>     m_MaterialDebug;

	std::shared_ptr<IGeometry>         m_PointBox;
	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};