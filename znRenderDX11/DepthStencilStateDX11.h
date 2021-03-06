#pragma once

class ZN_API DepthStencilStateDX11 
	: public DepthStencilStateBase
{
public:
	DepthStencilStateDX11(IRenderDeviceDX11& RenderDeviceDX11);
	DepthStencilStateDX11(const DepthStencilStateDX11& copy);
	virtual ~DepthStencilStateDX11();

	const DepthStencilStateDX11& operator=(const DepthStencilStateDX11& other);

    // DepthStencilState
    void Bind() override final;
	void Unbind() override final;

private:
	ATL::CComPtr<ID3D11DepthStencilState> m_pDepthStencilState;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};