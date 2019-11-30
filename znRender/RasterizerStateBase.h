#pragma once

class OW_ENGINE_API RasterizerStateBase : public IRasterizerState, public Object
{
public:
	RasterizerStateBase();
    virtual ~RasterizerStateBase();

	virtual void SetFillMode(FillMode frontFace = FillMode::Solid, FillMode backFace = FillMode::Solid);
	virtual void GetFillMode(FillMode& frontFace, FillMode& backFace) const;

	virtual void SetCullMode(CullMode cullMode = CullMode::Back);
	virtual CullMode GetCullMode() const;

	virtual void SetFrontFacing(FrontFace frontFace = FrontFace::CounterClockwise);
	virtual FrontFace GetFrontFacing() const;

	virtual void SetDepthBias(float depthBias = 0.0f, float slopeBias = 0.0f, float biasClamp = 0.0f);
	virtual void GetDepthBias(float& depthBias, float& slopeBias, float& biasClamp) const;

	virtual void SetDepthClipEnabled(bool depthClipEnabled = true);
	virtual bool GetDepthClipEnabled() const;

	virtual void SetViewport(const Viewport* viewport);
	virtual void SetViewports(const std::vector<const Viewport*>& viewports);
	virtual const std::vector<const Viewport *>& GetViewports();

	virtual void SetScissorEnabled(bool scissorEnable = false);
	virtual bool GetScissorEnabled() const;

	virtual void SetScissorRect(const Rect& rect);
	virtual void SetScissorRects(const std::vector<Rect>& rects);
	virtual const std::vector<Rect>& GetScissorRects() const;

	virtual void SetMultisampleEnabled(bool multisampleEnabled = false);
	virtual bool GetMultisampleEnabled() const;

	virtual void SetAntialiasedLineEnable(bool antialiasedLineEnable);
	virtual bool GetAntialiasedLineEnable() const;

	virtual void SetForcedSampleCount(uint8_t forcedSampleCount = 0);
	virtual uint8_t GetForcedSampleCount();

	virtual void SetConservativeRasterizationEnabled(bool conservativeRasterizationEnabled = false);
	virtual bool GetConservativeRasterizationEnabled() const;

protected:
    FillMode                                        m_FrontFaceFillMode;
    FillMode                                        m_BackFaceFillMode;

    CullMode                                        m_CullMode;

    FrontFace                                       m_FrontFace;

    float                                           m_DepthBias;
    float                                           m_SlopeBias;
    float                                           m_BiasClamp;

    bool                                            m_DepthClipEnabled;
    bool                                            m_ScissorEnabled;

    bool                                            m_MultisampleEnabled;
    bool                                            m_AntialiasedLineEnabled;

    bool                                            m_ConservativeRasterization;

    uint8_t                                         m_ForcedSampleCount;

    typedef std::vector<Rect>						RectList;
    RectList                                        m_ScissorRects;

    typedef std::vector<const Viewport*>			ViewportList;
    ViewportList                                    m_Viewports;

    bool                                            m_StateDirty;
    bool                                            m_ViewportsDirty;
    bool                                            m_ScissorRectsDirty;
};
