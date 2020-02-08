#pragma once

class ZN_API RenderPass 
	: public IRenderPass
	, public Object
{
public:
	RenderPass(IRenderDevice& RenderDevice);
	virtual ~RenderPass();

	// IRenderPass
	virtual void SetEnabled(bool Value) override final;
	virtual bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) = 0;
	virtual void PostRender(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit(const ISceneNode3D* node) override;
	virtual bool Visit(const ISceneNodeUI* node) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual bool Visit(const ILightComponent3D* light) override;

protected:
	const RenderEventArgs* GetRenderEventArgs() const;
    IRenderDevice& GetRenderDevice() const;
	const IBaseManager* GetBaseManager() const;

private:
	bool                                            m_Enabled;
    const RenderEventArgs*                          m_RenderEventArgs;
    IRenderDevice&									m_RenderDevice;
	const IBaseManager*                             m_BaseManager;
};