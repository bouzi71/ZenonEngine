#pragma once

#include "M2_CommonTypes.h"
#include "M2_SkinTypes.h"

// FORWARD BEGIN
class CM2;
class CM2_Base_Instance;
// FORWARD END

class CM2_SkinSection
	: public GeometryProxie
{
public:
	CM2_SkinSection(IRenderDevice& RenderDevice, const CM2& M2Model, const uint16 SkinSectionIndex, const SM2_SkinSection& SkinSectionProto, const std::vector<SM2_Vertex>& Vertexes, const std::vector<uint16>& Indexes);
	virtual ~CM2_SkinSection();

	void UpdateGeometryProps(const RenderEventArgs& RenderEventArgs, const CM2_Base_Instance* M2Instance);
	const std::shared_ptr<IConstantBuffer>& GetGeometryPropsBuffer() const;
	const std::shared_ptr<IStructuredBuffer>& GetGeometryBonesBuffer() const;

	uint16                  getIndex() const { return m_SkinSectionIndex; }
	const SM2_SkinSection&  getProto() const { return m_SkinSectionProto; }

	bool operator<(const CM2_SkinSection& other) const
	{
		return getIndex() < other.getIndex();
	}
	bool operator<(const CM2_SkinSection* other) const
	{
		return getIndex() < other->getIndex();
	}

private:
	const uint16            m_SkinSectionIndex;
	const SM2_SkinSection   m_SkinSectionProto;

private:
	__declspec(align(16)) struct ShaderM2GeometryProperties
	{
		ShaderM2GeometryProperties()
			: gIsAnimated(0)
			, gBonesMaxInfluences(0)
		{}

		uint32 gIsAnimated;
		uint32 gStartBoneIndex;
		uint32 gBonesMaxInfluences;
		float __padding;
	};
	ShaderM2GeometryProperties* m_Properties;
	std::shared_ptr<IConstantBuffer> m_PropertiesBuffer;

	std::vector<glm::mat4> m_BonesList;
	std::shared_ptr<IStructuredBuffer> m_StructuredBuffer;

private:
	IRenderDevice&          m_RenderDevice;
	const CM2&               m_M2Model;
};