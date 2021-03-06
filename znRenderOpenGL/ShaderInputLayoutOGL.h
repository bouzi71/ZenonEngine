#pragma once

class ShaderInputLayoutOGL : public ShaderInputLayoutBase
{
public:
	                                                ShaderInputLayoutOGL();
	virtual                                         ~ShaderInputLayoutOGL();

    // IShaderInputLayout
    bool                                            HasSemantic(const BufferBinding& binding) const override;
    const InputSemantic&                            GetSemantic(const BufferBinding& binding) const override;
    UINT                                            GetSemanticSlot(const BufferBinding& binding) const override;

	// ShaderInputLayoutOGL
	bool                                            LoadFromReflector(GLuint GLObj);
	bool                                            LoadFromCustomElements(GLuint GLObj, const std::vector<SCustomVertexElement>& CustomElements);

private:
};