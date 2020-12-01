#include "stdafx.h"

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(IScene& Scene)
	: ScenePassPipelined(Scene)
{
}

BaseUIPass::~BaseUIPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> BaseUIPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);
	
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult BaseUIPass::Visit(const IUIControl* sceneNode)
{
	BindPerObjectData(PerObject(sceneNode->GetWorldTransfom()));
	return EVisitResult::AllowAll;
}

EVisitResult BaseUIPass::Visit(const IModel * Model)
{
	Model->Render();
	return EVisitResult::AllowAll;
}

EVisitResult BaseUIPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	return EVisitResult::AllowAll;
}



//
// Protected
//
PerFrame BaseUIPass::GetPerFrameData() const
{
	const Viewport& viewport = GetPipeline().GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		glm::mat4(1.0f),
		viewport.GetOrthoMatix(), 
		glm::vec2(viewport.GetSize()),
		glm::vec3(0.0f)
	);
	return perFrame;
}
