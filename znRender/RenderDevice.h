#pragma once

#include "common.h"

#include "RenderEvents.h"
#include "CPUAccess.h"

class IBuffer;
class ConstantBuffer;
class StructuredBuffer;
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
class IRenderTarget;
#include "Query.h"
class SamplerState;
#include "Material.h"
class PipelineState;

class OW_ENGINE_API IRenderDevice : public Object
{
public:
	// Create an vertex buffer (with std::vector)
	template<typename T>
	std::shared_ptr<IBuffer> CreateVertexBuffer(const T& data);
	template<typename T>
	std::shared_ptr<IBuffer> CreateVertexBuffer(const T* data, uint32 count);
	virtual void DestroyVertexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	// Create an index buffer.
	template<typename T>
	std::shared_ptr<IBuffer> CreateIndexBuffer(const T& data);
	template<typename T>
	std::shared_ptr<IBuffer> CreateIndexBuffer(const T* data, uint32 count);
	virtual void DestroyIndexBuffer(std::shared_ptr<IBuffer> buffer) = 0;

	// Create a constant buffer (or Uniform buffer)
	template<typename T>
	std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const T& data);
	virtual void DestroyConstantBuffer(std::shared_ptr<ConstantBuffer> buffer) = 0;

	// Create a StructuredBuffer
	template<typename T>
	std::shared_ptr<StructuredBuffer> CreateStructuredBuffer(const std::vector<T>& data, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false);
	virtual void DestroyStructuredBuffer(std::shared_ptr<StructuredBuffer> buffer) = 0;

	
	std::shared_ptr<IMesh> CreateLine(cvec3 _dest); // Create a line
	std::shared_ptr<IMesh> CreatePlane(cvec3 N = vec3(0, 1, 0)); // Create a plane in 3D.
	std::shared_ptr<IMesh> CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f); // Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	std::shared_ptr<IMesh> CreateSphere(); // Create a sphere in 3D
	std::shared_ptr<IMesh> CreateCube(); // Create a cube in 3D.
	std::shared_ptr<IMesh> CreateCone(); // Create a cone.

    // 3D meshes
    std::shared_ptr<IMesh> Create3DQuad(float width = 10.0f, float height = 10.0f);
    std::shared_ptr<IMesh> Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f));

    // UI meshes
    std::shared_ptr<IMesh> CreateUIQuad(float width = 10.0f, float height = 10.0f);
    std::shared_ptr<IMesh> CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f));



	//
	// ABSTRACT
	//

	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;

    virtual const std::string& GetDeviceName() const = 0;
    virtual const RenderDeviceType GetDeviceType() const = 0;
	virtual const std::shared_ptr<IBaseManager>& GetBaseManager() const = 0;
	virtual std::shared_ptr<RenderWindow> CreateRenderWindow(IWindowObject * WindowObject, bool vSync) = 0;

	virtual void Lock() = 0;
	virtual void Unlock() = 0;

	virtual std::shared_ptr<IMesh> CreateMesh() = 0;
	virtual void DestroyMesh(std::shared_ptr<IMesh> mesh) = 0;

	virtual std::shared_ptr<Shader> CreateShader(Shader::ShaderType type, const std::string& fileName, const Shader::ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout = nullptr) = 0;
	virtual void DestroyShader(std::shared_ptr<Shader> shader) = 0;

	// Create a texture from a file.
	virtual std::shared_ptr<Texture> CreateTexture2D(const std::string& fileName) = 0;
	virtual std::shared_ptr<Texture> CreateTextureCube(const std::string& fileName) = 0;

	// Create an empty texture of a predefined size.
	virtual std::shared_ptr<Texture> CreateTexture2D(uint16_t width, uint16_t height, uint16_t slices, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<Texture> CreateTextureCube(uint16_t size, uint16_t numCubes = 1, const Texture::TextureFormat& format = Texture::TextureFormat(), CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;
	virtual std::shared_ptr<Texture> GetDefaultTexture() const = 0;

	// Create an null texture (can be loaded later using Texture::LoadTexture2D function.
	virtual std::shared_ptr<Texture> CreateTexture() = 0;
	virtual void DestroyTexture(std::shared_ptr<Texture> texture) = 0;

	// Create an material
	virtual std::shared_ptr<IMaterial> CreateMaterial(size_t Size) = 0;
	virtual void DestroyMaterial(std::shared_ptr<IMaterial> material) = 0;

	// Create a render target
	virtual std::shared_ptr<IRenderTarget> CreateRenderTarget() = 0;
	virtual void DestroyRenderTarget(std::shared_ptr<IRenderTarget> renderTarget) = 0;

	// Create a GPU query object. Used for performance profiling, occlusion queries, or primitive output queries.
	virtual std::shared_ptr<Query> CreateQuery(Query::QueryType queryType = Query::QueryType::Timer, uint8_t numBuffers = 3) = 0;
	virtual void DestoryQuery(std::shared_ptr<Query> query) = 0;

	virtual std::shared_ptr<SamplerState> CreateSamplerState() = 0;
	virtual void DestroySampler(std::shared_ptr<SamplerState> sampler) = 0;

	virtual std::shared_ptr<PipelineState> CreatePipelineState() = 0;
	virtual void DestoryPipelineState(std::shared_ptr<PipelineState> pipeline) = 0;

	virtual std::shared_ptr<IBuffer> CreateVoidVertexBuffer(const void* data, uint32 count, uint32 offset, uint32 stride) = 0;

	virtual std::shared_ptr<IBuffer> CreateUInt16IndexBuffer(const uint16* data, uint32 sizeInBytes) = 0;
	virtual std::shared_ptr<IBuffer> CreateUInt32IndexBuffer(const uint32* data, uint32 sizeInBytes) = 0;

	virtual std::shared_ptr<ConstantBuffer> CreateConstantBuffer(const void* data, size_t size) = 0;
	virtual std::shared_ptr<StructuredBuffer> CreateStructuredBuffer(void* data, uint32 count, uint32 stride, CPUAccess cpuAccess = CPUAccess::None, bool gpuWrite = false) = 0;

protected:
    virtual void                                    CreateDevice() = 0;

protected:
    typedef std::vector< std::shared_ptr<IBuffer> > BufferList;
    BufferList                                      m_Buffers;

    typedef std::vector< std::shared_ptr<IMesh> > MeshList;
    MeshList                                        m_Meshes;

    typedef std::vector< std::shared_ptr<Shader> > ShaderList;
    typedef std::map< std::string, std::shared_ptr<Shader> > ShaderNameMap;
    ShaderList                                      m_Shaders;
    ShaderNameMap                                     m_ShadersByName;

    typedef std::vector< std::shared_ptr<Texture> > TextureList;
    typedef std::map< std::string, std::shared_ptr<Texture> > TextureMap;
    TextureList                                     m_Textures;
    TextureMap                                      m_TexturesByName;
    std::shared_ptr<Texture>                        m_pDefaultTexture;

    typedef std::vector< std::shared_ptr<IMaterial> > MaterialList;
    MaterialList                                    m_Materials;

    typedef std::vector< std::shared_ptr<IRenderTarget> > RenderTargetList;
    RenderTargetList                                m_RenderTargets;

    typedef std::vector< std::shared_ptr<SamplerState> > SamplerList;
    SamplerList                                     m_Samplers;

    typedef std::vector< std::shared_ptr<PipelineState> > PipelineList;
    PipelineList                                    m_Pipelines;

    typedef std::vector< std::shared_ptr<Query> > QueryList;
    QueryList                                       m_Queries;
};

#include "RenderDevice.inl"