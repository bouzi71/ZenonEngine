#pragma once

class ZN_API CGameState_World : public CGameState
{
public:
	CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents);
	virtual ~CGameState_World();

	// IGameState
	bool Init();
	void Destroy();

	virtual void OnRayIntersected(const glm::vec3& Point) override;

	virtual void OnResize(ResizeEventArgs& e) override;

	virtual void OnPreRender(RenderEventArgs& e) override;
	virtual void OnRender(RenderEventArgs& e) override;
	virtual void OnPostRender(RenderEventArgs& e) override;
	virtual void OnRenderUI(RenderEventArgs& e) override;

private:
	void Load3D();
	void LoadUI();
	void LoadObj(const std::string& ObjFilename);
};