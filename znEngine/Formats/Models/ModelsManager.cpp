#include "stdafx.h"

// General
#include "ModelsManager.h"

CznModelsManager::CznModelsManager(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
}

CznModelsManager::~CznModelsManager()
{
}



//
// IznModelsManager
//
void CznModelsManager::AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder)
{
	_ASSERT(std::find_if(m_ModelsLoaders.begin(), m_ModelsLoaders.end(), [&ModelsLaoder] (const std::shared_ptr<IznModelsLoader>& Loader) ->  bool {
		return Loader->GetName() == ModelsLaoder->GetName();
	}) == m_ModelsLoaders.end());
	m_ModelsLoaders.push_back(ModelsLaoder);
}

void CznModelsManager::RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder)
{
	const auto& it = std::find_if(m_ModelsLoaders.begin(), m_ModelsLoaders.end(), [&ModelsLaoder](const std::shared_ptr<IznModelsLoader>& Loader) ->  bool {	return Loader->GetName() == ModelsLaoder->GetName(); });
	if (it == m_ModelsLoaders.end())
		return;
	m_ModelsLoaders.erase(it);
}

std::shared_ptr<IModel> CznModelsManager::LoadModel(const std::string& ModelFileName, const std::shared_ptr<IznLoaderParams>& LoaderParams)
{
	// Find existsing cached
	const auto& iter = m_ModelsByName.find(ModelFileName);
	if (iter != m_ModelsByName.end())
	{
		if (auto model = iter->second.lock())
		{
			return model;
		}
		else
		{
			m_ModelsByName.erase(iter);
		}
	}

	auto file = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (file == nullptr)
		throw CException("Model file '%s' not found.", ModelFileName.c_str());

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(file))
		{
			std::shared_ptr<IModel> model = loader->LoadModel(file, LoaderParams);
			m_ModelsByName[ModelFileName] = model;
			return model;
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", ModelFileName.c_str());
}

std::shared_ptr<IFile> CznModelsManager::SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName)
{
	_ASSERT(Model != nullptr);
	_ASSERT(FileName.size() > 0);
	
	auto file = m_BaseManager.GetManager<IFilesManager>()->Create(FileName);

	for (const auto& loader : m_ModelsLoaders)
	{
		if (loader->IsSupportedFormat(file))
		{
			return loader->SaveModel(Model, FileName);
		}
	}

	throw CException("The loader for model '%s' doesn't exists.", FileName.c_str());
}
