#pragma once

#include "common.h"

#include "Shader.h"
#include "Texture.h"
#include "SamplerState.h"

class OW_ENGINE_API Material : public Object, public std::enable_shared_from_this<Material>
{
public:
	typedef std::map<Shader::ShaderType, std::shared_ptr<Shader>> ShaderMap;
	typedef std::map<uint8, std::shared_ptr<Texture>> TextureMap;
    typedef std::map<uint8, std::shared_ptr<SamplerState>> SamplersMap;

public:
	virtual void SetShader(Shader::ShaderType type, std::shared_ptr<Shader> pShader) = 0;
	virtual std::shared_ptr<Shader> GetShader(Shader::ShaderType type) const = 0;
	virtual const ShaderMap& GetShaders() const = 0;

	virtual std::shared_ptr<Texture> GetTexture(uint8 ID) const = 0;
	virtual void SetTexture(uint8 ID, std::shared_ptr<Texture> texture) = 0;

    virtual std::shared_ptr<SamplerState> GetSampler(uint8 ID) const = 0;
    virtual void SetSampler(uint8 ID, std::shared_ptr<SamplerState> samplerState) = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetWrapper(std::weak_ptr<Material> _wrapper) = 0;
	virtual void CreateConstantBuffer(const void* data, size_t size) = 0;
	virtual void UpdateConstantBuffer() const = 0;
	virtual void UpdateConstantBuffer(const void* _data, size_t size) const = 0;
	virtual void MarkConstantBufferDirty() = 0;
};