#pragma once

class ZN_API BlendStateDX11 : public BlendStateBase
{
public:
	                                                BlendStateDX11(ID3D11Device2* pDevice);
	                                                BlendStateDX11(const BlendStateDX11& copy);
	virtual                                         ~BlendStateDX11();

	const BlendStateDX11&                           operator=(const BlendStateDX11& other);

    // BlendState
	void                                            Bind() override final;

private:
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
	ATL::CComPtr<ID3D11BlendState1>                 m_pBlendState;
};