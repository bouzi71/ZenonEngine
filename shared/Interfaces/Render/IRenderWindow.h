#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderTarget;
ZN_INTERFACE IznNativeWindow;
class Viewport;
// FORWARD END



ZN_INTERFACE ZN_API IRenderWindowEventListener
{
	virtual ~IRenderWindowEventListener() {}

	virtual void OnUpdate(UpdateEventArgs& e) = 0;
	virtual void OnRender(RenderEventArgs& e) = 0;
};



ZN_INTERFACE ZN_API IRenderWindow
	: public IznNativeWindow
	//, public std::enable_shared_from_this<IRenderWindow>
{
	virtual ~IRenderWindow() {}

	virtual bool IsVSync() const = 0;
	virtual void Present() = 0;

	virtual void SetRenderWindowEventListener(std::shared_ptr<IRenderWindowEventListener> RenderWindowEventListener) = 0;
	virtual void SetNativeWindowEventListener(std::shared_ptr<IznNativeWindowEventListener> NativeWindowEventListener) = 0;
	virtual const std::shared_ptr<IRenderTarget>& GetRenderTarget() const = 0;
	virtual const Viewport& GetViewport() const = 0;

	virtual float GetUpdateDeltaTime() const = 0;
	virtual float GetRenderDeltaTime() const = 0;
	virtual float GetSummaDeltaTime() const = 0;
};
