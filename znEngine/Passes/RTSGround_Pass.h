#pragma once

#include "Scene/SceneCreateTypelessListPass.h"

class ZN_API CRTSGround_Pass
	: public Base3DPass
{
public:
	CRTSGround_Pass(IRenderDevice& RenderDevice, IScene& scene);
	virtual ~CRTSGround_Pass();

	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override;

private:
	IShaderParameter* m_ShaderLightsBufferParameter;

	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};