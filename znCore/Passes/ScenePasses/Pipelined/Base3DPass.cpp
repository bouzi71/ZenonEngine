#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IScene& scene)
    : ScenePassPipelined(scene)
{

}

Base3DPass::~Base3DPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> Base3DPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult Base3DPass::Visit(const ISceneNode* SceneNode)
{
	if (false == SceneNode->IsEnabled())
		return EVisitResult::Block;

	BindPerObjectData(PerObject(SceneNode->GetWorldTransfom()));
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const IModel * Model)
{
	Model->Render();
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	return EVisitResult::AllowAll;
}

