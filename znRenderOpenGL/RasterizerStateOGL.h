#pragma once

class RasterizerStateOGL : public RasterizerStateBase
{
public:
	                                                RasterizerStateOGL();
	                                                RasterizerStateOGL(const RasterizerStateOGL& copy);
	virtual                                         ~RasterizerStateOGL();

	const RasterizerStateOGL&                       operator=(const RasterizerStateOGL& other);

    // RasterizerState
	void                                            Bind() override final;
};