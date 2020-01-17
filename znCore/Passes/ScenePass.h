#pragma once

#include "RenderPass.h"

class ZN_API ScenePass
	: public RenderPass
{
public:
	ScenePass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~ScenePass();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

protected:
	std::shared_ptr<IScene> GetScene() const;
	
private:
	std::shared_ptr<IScene> m_Scene;
};