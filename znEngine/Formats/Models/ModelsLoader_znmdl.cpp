#include "stdafx.h"

// General
#include "ModelsLoader_znmdl.h"

// Additional
#include "ModelsLoaderHelper.h"
#include "Scene/Components/Animations/Animation.h"
#include "Scene/Components/ModelComponent/Skeleton.h"

namespace
{
	const char * cLoaderSupportedExtension = "znmdl";
}

CModelsLoader_znmdl::CModelsLoader_znmdl(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CModelsLoader_znmdl::~CModelsLoader_znmdl()
{
}



//
// IznModelsLoader
//
std::string CModelsLoader_znmdl::GetName() const
{
	return "ModelsLoader_znmdl";
}

bool CModelsLoader_znmdl::IsSupportedFormat(const std::string & ModelFileName) const
{
	std::string lowerFileName = Utils::ToLower(ModelFileName);
	std::string extension = lowerFileName;
	if (ModelFileName.length() > strlen(cLoaderSupportedExtension))
		extension = extension.substr(extension.length() - strlen(cLoaderSupportedExtension));
	return extension == cLoaderSupportedExtension;
}

bool CModelsLoader_znmdl::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == cLoaderSupportedExtension;
}

std::shared_ptr<IModel> CModelsLoader_znmdl::LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const
{
	auto modelFile = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (modelFile == nullptr)
		throw CException("Model file '%s' not found.", ModelFileName.c_str());
	return LoadModel(modelFile, LoaderParams);
}

std::shared_ptr<IModel> CModelsLoader_znmdl::LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const
{
	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
	
	ModelFile->seek(0);

	auto model = renderDevice.GetObjectsFactory().CreateModel();
	auto modelInternal = std::dynamic_pointer_cast<IModelInternal>(model);

	modelInternal->SetFileName(ModelFile->Path_Name());

	//
	// Common data
	BoundingBox bounds;
	bounds.Load(ModelFile);
	modelInternal->SetBounds(bounds);


	//
	// Load materials
	std::vector<std::shared_ptr<IMaterial>> materials;
	size_t materialsCount;
	ModelFile->read(&materialsCount);
	materials.reserve(materialsCount);
	for (size_t i = 0; i < materialsCount; i++)
	{
		auto material = m_BaseManager.GetManager<IMaterialsFactory>()->CreateMaterial("MaterialModel");
		std::dynamic_pointer_cast<IObjectLoadSave>(material)->Load(ModelFile);
		m_BaseManager.GetManager<IMaterialsFactory>()->AddMaterial(material);
		materials.push_back(material);
	}


	//
	// Load geometries
	std::vector<std::shared_ptr<IGeometry>> geometries;
	size_t geometriesCount;
	ModelFile->read(&geometriesCount);
	geometries.reserve(geometriesCount);
	for (size_t i = 0; i < geometriesCount; i++)
	{
		auto geometry = renderDevice.GetObjectsFactory().CreateGeometry();
		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(geometry))
		{
			geometryAsLoadableFromFile->Load(ModelFile);
		}
		else
			throw CException("Geometry '%s' is not supports 'IObjectLoadSave'.", geometry->GetName().c_str());

		geometries.push_back(geometry);
	}


	//
	// Load connections
	{
		size_t indexedConnectionsCount;
		ModelFile->read(&indexedConnectionsCount);
		for (size_t i = 0; i < indexedConnectionsCount; i++)
		{
			size_t materialIndex;
			ModelFile->read(&materialIndex);

			size_t geometryIndex;
			ModelFile->read(&geometryIndex);

			SGeometryDrawArgs drawArgs;
			ModelFile->read(&drawArgs);

			auto materialObject = materials[materialIndex];
			auto geometryObject = geometries[geometryIndex];

			model->AddConnection(materialObject, geometryObject, drawArgs);
		}
	}

	//
	// Load animations
	{
		size_t animationsCount;
		if (ModelFile->read(&animationsCount))
		{
			for (size_t i = 0; i < animationsCount; i++)
			{
				std::shared_ptr<IAnimation> anim = MakeShared(CAnimation, ModelFile);
				model->AddAnimation(anim->GetName(), anim);
			}
		}
	}


	//
	// Skeleton
	{
		uint32 isSkeletonExists;
		ModelFile->read(&isSkeletonExists);

		if (isSkeletonExists)
		{
			auto skeleton = MakeShared(CSkeleton);
			std::dynamic_pointer_cast<IObjectLoadSave>(skeleton)->Load(ModelFile);

			modelInternal->SetSkeleton(skeleton);
		}
	}

	return model;
}

std::shared_ptr<IFile> CModelsLoader_znmdl::SaveModel(const std::shared_ptr<IModel>& Model, const std::string & FileName) const
{	
	std::vector<std::shared_ptr<IMaterial>> materials;
	std::vector<std::shared_ptr<IGeometry>> geometries;
	std::vector<ModelsLoaderHelper::SConnectionIndexed> indexedConnections;

	const auto& modelConnections = Model->GetConnections();
	for (const auto& it : modelConnections)
	{
		size_t materialIndex = ModelsLoaderHelper::FindOrAddMaterialIndex(materials, it.Material);
		size_t geometryIndex = ModelsLoaderHelper::FindOrAddGeometryIndex(geometries, it.Geometry);
		indexedConnections.push_back(ModelsLoaderHelper::SConnectionIndexed{ materialIndex, geometryIndex, it.GeometryDrawArgs });
	}

	auto file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);


	//
	// Common data
	BoundingBox modelBBox = Model->GetBounds();
	modelBBox.Save(file);


	//
	// Save materials
	size_t materialsCount = materials.size();
	file->write(&materialsCount);
	for (const auto& m : materials)
	{
		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m))
		{
			materialAsLoadableFromFile->Save(file);
		}
		else
			throw CException("Material '%s' is not supports 'IObjectLoadSave'.", m->GetName().c_str());
	}


	//
	// Save geometries
	size_t geometriesCount = geometries.size();
	file->write(&geometriesCount);
	for (const auto& g : geometries)
	{
		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(g))
		{
			geometryAsLoadableFromFile->Save(file);
		}
		else
			throw CException("Geometry '%s' is not supports 'IObjectLoadSave'.", g->GetName().c_str());
	}


	//
	// Save connections
	{
		size_t connectionsCount = indexedConnections.size();
		file->write(&connectionsCount);
		for (const auto& c : indexedConnections)
		{
			file->write(&c.MaterialIndex);
			file->write(&c.GeometryIndex);
			file->write(&c.GeometryDrawArgs);
		}
	}


	//
	// Save animations
	{
		const auto& animations = Model->GetAnimations();
		size_t animationsCount = animations.size();
		file->write(&animationsCount);
		for (const auto& a : animations)
		{
			if (auto loadSave = std::dynamic_pointer_cast<IObjectLoadSave>(a.second))
				loadSave->Save(file);
		}
	}

	
	//
	// Skeleton
	{
		uint32 isSkeletonExists = Model->GetSkeleton() != nullptr;
		file->write(&isSkeletonExists);

		if (isSkeletonExists)
			std::dynamic_pointer_cast<IObjectLoadSave>(Model->GetSkeleton())->Save(file);
	}

	return file;
}
