#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXSceneNode.h"



class CFBXScene : public std::enable_shared_from_this<CFBXScene>
{
public:
	CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxScene* NativeScene);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);
	bool SaveToFile(std::shared_ptr<IFile> File, int pFileFormat = -1, bool pEmbedMedia = false);

	bool LoadNodes(ISceneNode3D* ParentNode);

public:
	fbxsdk::FbxScene*   GetNativeScene() const;
	fbxsdk::FbxManager* GetNativeManager() const;
	std::string         GetPath() const;
	CFBXSceneNode* GetRootNode() const;

private:
	std::string m_Path;
	fbxsdk::FbxScene* m_NativeScene;

	CFBXSceneNode* m_RootNode;

private:
	const IBaseManager& m_BaseManager;
};

#endif
