#include "stdafx.h"

// General
#include "M2.h"

CM2::CM2(IBaseManager& BaseManager, IRenderDevice& RenderDevice, const std::string& FileName)
	: m_UniqueName("")

	// Loops and sequences
	, m_IsAnimated(false)

	, m_BaseManager(BaseManager)
	, m_RenderDevice(RenderDevice)
{
	// 1. Fix filename ('*.MDX' -> '*.M2')
	std::string fixedFileName = FileName;
	if (fixedFileName.back() != '2')
	{
		fixedFileName[fixedFileName.length() - 2] = '2';
		fixedFileName[fixedFileName.length() - 1] = '\0';
		fixedFileName.resize(fixedFileName.length() - 1);
	}

	// 2. Try open file and initialize internal paths
	m_F = BaseManager.GetManager<IFilesManager>()->Open(fixedFileName);
	if (m_F == nullptr)
		throw CException("M2: Model file not found '" + fixedFileName + "'.");

	// 3. Read header and important data
	m_F->read(&m_Header);

	//_ASSERT(std::strcmp(m_Header.magic, "MD20") == 0);
	//_ASSERT(m_Header.version >= 256 && m_Header.version <= 257);
	//_ASSERT(m_Header.version >= 260 && m_Header.version <= 263);
	
	if (m_Header.name.size > 0)
		m_UniqueName = std::string((const char*)(m_F->getData() + m_Header.name.offset));

	m_Bounds = m_Header.bounding_box.Convert();
}

CM2::~CM2()
{
}

std::shared_ptr<CM2_Skin> CM2::GetSkin() const
{
	return m_Skin;
}

bool CM2::Load()
{
	// Skeleton
	m_Skeleton = std::make_unique<CM2_Comp_Skeleton>(*this);
	m_Skeleton->Load(m_Header, m_F);

	// Colors, textures and etc...
	m_Materials = std::make_unique<CM2_Comp_Materials>(*this);
	m_Materials->Load(m_Header, m_F);

	// Lights, cameras, attachments
	m_Miscellaneous = std::make_unique<CM2_Comp_Miscellaneous>(*this);
	m_Miscellaneous->Load(m_Header, m_F);

	// Skins
	if (m_Header.vertices.size > 0)
	{
		// Vertices
		std::vector<SM2_Vertex>	m2Vertexes;
		const SM2_Vertex* Vertexes = (const SM2_Vertex*)(m_F->getData() + m_Header.vertices.offset);
		for (uint32 i = 0; i < m_Header.vertices.size; i++)
			m2Vertexes.push_back(Vertexes[i]);

		for (uint32 i = 0; i < m_Header.vertices.size; i++)
		{
			m2Vertexes[i].pos = Fix_XZmY(m2Vertexes[i].pos);
			m2Vertexes[i].normal = Fix_XZmY(m2Vertexes[i].normal);
		}

#if WOW_CLIENT_VERSION <= WOW_BC_2_4_3
		_ASSERT(m_Header.skin_profiles.size > 0);
		const SM2_SkinProfile* m2Skins = (const SM2_SkinProfile*)(m_F->getData() + m_Header.skin_profiles.offset);
		for (uint32 i = 0; i < m_Header.skin_profiles.size; i++)
		{
			std::shared_ptr<CM2_Skin> skin = std::make_shared<CM2_Skin>(m_BaseManager, m_RenderDevice, *this, m2Skins[i]);
			skin->Load(m_Header, m_F, m2Vertexes);
			m_Skin = skin;
			break;
		}
#else
		_ASSERT(m_Header.num_skin_profiles > 0);
		for (uint32 i = 0; i < m_Header.num_skin_profiles; i++)
		{
			char buf[256];
			sprintf_s(buf, "%s%02d.skin", getFilenameWithoutExt().c_str(), i);

			std::shared_ptr<IFile> skinFile = GetBaseManager().GetManager<IFilesManager>()->Open(buf);


			const SM2_SkinProfile* m2Skin = (const SM2_SkinProfile*)skinFile->getData();

			std::shared_ptr<CM2_Skin> skin = std::make_shared<CM2_Skin>(m_BaseManager, m_RenderDevice, *this, *m2Skin);
			skin->Load(m_Header, skinFile, m2Vertexes);
			m_Skin = skin;
			break;
		}
#endif
	}
	else
	{
#if WOW_CLIENT_VERSION <= WOW_BC_2_4_3
		Log::Warn("M2[%s] don't contain geometry. Skins [%d]", getFilename().c_str(), m_Header.skin_profiles.size);
#else
		Log::Warn("M2[%s] don't contain geometry. Skins [%d]", getFilename().c_str(), m_Header.num_skin_profiles);
#endif
	}

#if 0
	// Collisions
	std::shared_ptr<IBuffer> collisonVB = nullptr;
	std::shared_ptr<IBuffer> collisonIB = nullptr;
	if (m_Header.collisionVertices.size > 0)
	{
		std::vector<vec3> collisionVertices;
		vec3* CollisionVertices = (vec3*)(m_F->getData() + m_Header.collisionVertices.offset);
		for (uint32 i = 0; i < m_Header.collisionVertices.size; i++)
		{
			collisionVertices.push_back(CollisionVertices[i]);
		}

		for (uint32 i = 0; i < m_Header.collisionVertices.size; i++)
		{
			collisionVertices[i] = Fix_XZmY(collisionVertices[i]);
		}

		collisonVB = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(collisionVertices);
	}

	if (m_Header.collisionTriangles.size > 0)
	{
		std::vector<uint16> collisionTriangles;
		uint16* CollisionTriangles = (uint16*)(m_F->getData() + m_Header.collisionTriangles.offset);
		for (uint32 i = 0; i < m_Header.collisionTriangles.size; i++)
		{
			collisionTriangles.push_back(CollisionTriangles[i]);
		}

		collisonIB = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(collisionTriangles);
	}

	if (collisonVB != nullptr && collisonIB != nullptr)
	{
		m_CollisionVetCnt = m_Header.collisionVertices.size;
		m_CollisionIndCnt = m_Header.collisionTriangles.size;

		//m_M2->m_CollisionGeom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_P);
		//m_M2->m_CollisionGeom->setGeomVertexParams(collisonVB, R_DataType::T_FLOAT, 0, sizeof(vec3)); // pos 0-2
		//m_M2->m_CollisionGeom->setGeomIndexParams(collisonIB, R_IndexFormat::IDXFMT_16);
		//m_M2->m_CollisionGeom->finishCreatingGeometry();
	}
	else
	{
		m_CollisionGeom = nullptr;
	}
#endif

	m_IsAnimated = getSkeleton().isAnimBones() || getSkeleton().isBillboard() || getMaterials().IsAnimTextures() || getMiscellaneous().IsAnimated() || true;
	_ASSERT(m_F.use_count() == 1);
	m_F.reset();

	return true;
}

bool CM2::Delete()
{
	return true;
}
