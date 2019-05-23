#pragma once


class MeshWrapper : public IMesh 
{
protected:
	typedef MeshWrapper base;
public:
	MeshWrapper(std::shared_ptr<IMesh> _mesh);
	virtual ~MeshWrapper();

	virtual void AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer);
	virtual void SetVertexBuffer(std::shared_ptr<IBuffer> buffer);
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> buffer);

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual void SetMaterial(std::shared_ptr<const Material> material);
	virtual std::shared_ptr<const Material> GetMaterial() const;

	virtual bool Render(const RenderEventArgs* renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override;

	virtual bool Accept(std::shared_ptr<IVisitor> visitor, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);

private:
	std::shared_ptr<IMesh> m_Mesh;
};