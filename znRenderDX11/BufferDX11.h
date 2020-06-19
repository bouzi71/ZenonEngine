#pragma once

class BufferDX11 
	: public IBuffer
	, public ILoadableFromFile
{
public:
	BufferDX11(IRenderDeviceDX11& RenderDeviceDX11, UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride);
	virtual ~BufferDX11();

	// IBuffer
	bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void Copy(const IBuffer* other) const override;
	BufferType GetType() const override;
	uint32 GetElementCount() const override;
	uint32 GetElementStride() const override;
    uint32 GetElementOffset() const override;

	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;

private:
	void InitializeInternal(UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride);

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;
	
	typedef std::vector<uint8_t> BufferType;
	BufferType m_Data;
	UINT m_Count; // The number of elements in this buffer.
	UINT m_Offset; // The offset of the vertex buffer in bytes.
	UINT m_Stride; // The stride of the vertex buffer in bytes.
	UINT m_BindFlags; // How this buffer should be bound.
	

private: // Link to parent
	IRenderDeviceDX11& m_RenderDeviceDX11;
};
