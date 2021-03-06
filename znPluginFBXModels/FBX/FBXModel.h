#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

namespace
{
	struct FBXVertex
	{
		FBXVertex()
		{
			controlPointIndex = UINT32_MAX;
			memset(weights, 0x00, 32);
			memset(indexes, 0x00, 32);
			bonesCnt = 0;
		}

		// Render part
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
		//glm::vec3 tangent;
		//glm::vec3 binormal;
		float weights[8];
		uint32 indexes[8];

		// Engine part
		uint32 controlPointIndex;
		size_t bonesCnt;
		//glm::vec3 _unused;
	};
}

class ZN_API CFBXModel
	: public ModelProxie
	, public IFBXModel
{
public:
	CFBXModel(const IBaseManager& BaseManager, const IFBXNode& FBXNode);
	virtual ~CFBXModel();

	bool Load(fbxsdk::FbxMesh* NativeMesh);

	// IFBXModel
	const IFBXNode& GetOwner() const override;
	std::shared_ptr<IModel> GetModel() override;
	
private:
	void MaterialLoad(fbxsdk::FbxMesh* NativeMesh);
	void SkeletonLoad(fbxsdk::FbxMesh* NativeMesh);

private:
	std::vector<FBXVertex> m_Vertices;
	bool m_HasBoneWeights;
	std::shared_ptr<IGeometry> m_Geometry;

	const IBaseManager& m_BaseManager;
	const IFBXNode& m_FBXNode;
};

#endif
