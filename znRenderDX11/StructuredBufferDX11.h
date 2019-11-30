#pragma once

class StructuredBufferDX11 : public IStructuredBuffer
{
public:
	StructuredBufferDX11(ID3D11Device2* pDevice, UINT bindFlags, const void* data, size_t count, UINT stride, CPUAccess cpuAccess = CPUAccess::None, bool bUAV = false);
	virtual ~StructuredBufferDX11();

	virtual bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;

	// Is this an index buffer or an attribute/vertex buffer?
	virtual BufferType GetType() const;
	// How many elements does this buffer contain?
	virtual uint32 GetElementCount() const;
	// Elements stride
	virtual uint32 GetElementStride() const;
    // Elements stride
    virtual uint32 GetElementOffset() const;

	virtual void Copy(std::shared_ptr<IStructuredBuffer> other);

	// Clear the contents of the buffer.
	virtual void Clear();

	// Used by the RenderTargetDX11 only.
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const;

protected:
	virtual void Copy(std::shared_ptr<IBuffer> other);
	virtual void SetData(void* data, size_t elementSize, size_t offset, size_t numElements);

	// Commit the data from system memory to device memory.
	void Commit() const;

private:
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;
	ATL::CComPtr<ID3D11ShaderResourceView> m_pSRV;
	ATL::CComPtr<ID3D11UnorderedAccessView> m_pUAV;

	// The system data buffer.
	typedef std::vector<uint8_t> BufferType;
	BufferType m_Data;

	// The stride of the vertex buffer in bytes.
	UINT m_uiStride;

	// How this buffer should be bound.
	UINT m_BindFlags;

	// The number of elements in this buffer.
	UINT m_uiCount;

	// The last slot the UAV was bound to.
	UINT m_uiSlot;

	// Marked dirty if the contents of the buffer differ
	// from what is stored on the GPU.
	mutable bool m_bIsDirty;
	// Does this buffer require GPU write access 
	// If so, it must be bound as a UAV instead of an SRV.
	bool m_bUAV;

	// Requires CPU read/write access.
	bool m_bDynamic;
	CPUAccess m_CPUAccess;
};
