#pragma once

#include "BaseManager.h"

// FORWARD BEGIN
ZN_INTERFACE IModel;
// FORWARD END


ZN_INTERFACE ZN_API IznModelsLoader
{
	virtual ~IznModelsLoader() {}

	virtual std::string GetName() const = 0;
	virtual bool IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::string& TexturesPath = "") const = 0;
};


ZN_INTERFACE ZN_API
	__declspec(uuid("ED37C829-7EC4-4EB9-9B62-524855EF8CFE"))
	IznModelsManager : public IManager
{
	virtual ~IznModelsManager() {};

	virtual void AddModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) = 0;
	virtual void RemoveModelsLoader(const std::shared_ptr<IznModelsLoader>& ModelsLaoder) = 0;
	virtual std::shared_ptr<IModel> LoadModel(const std::string& ModelFileName, const std::string& TexturesPath = "") = 0;
	virtual std::shared_ptr<IFile> SaveModel(const std::shared_ptr<IModel>& Model, const std::string& FileName) = 0;
};