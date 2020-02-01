#pragma once

#include "RenderPassPipelined.h"

class ZN_API ScenePassPipelined
	: public RenderPassPipelined
{
public:
	ScenePassPipelined(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~ScenePassPipelined();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

protected:
	std::shared_ptr<IScene> GetScene() const;
	
private:
	std::shared_ptr<IScene> m_Scene;
};