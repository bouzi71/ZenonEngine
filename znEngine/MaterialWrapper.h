#pragma once

class OW_ENGINE_API MaterialWrapper : public IMaterial
{
public:
	MaterialWrapper(std::shared_ptr<IMaterial> _materal);
	virtual ~MaterialWrapper();

	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader);
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const;
	virtual const ShaderMap& GetShaders() const;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const;
	virtual void SetTexture(uint8 type, std::shared_ptr<Texture> texture);

    virtual std::shared_ptr<SamplerState> GetSampler(uint8 ID) const;
    virtual void SetSampler(uint8 ID, std::shared_ptr<SamplerState> samplerState);

	virtual void Bind(const ShaderMap& shaders) const;
	virtual void Unbind(const ShaderMap& shaders) const;

	virtual void SetWrapper(std::weak_ptr<IMaterial> _wrapper);
	virtual void UpdateConstantBuffer() const;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const;
	virtual void MarkConstantBufferDirty();

private:
	std::shared_ptr<IMaterial> m_Material;
};