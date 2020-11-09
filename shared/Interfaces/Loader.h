#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE ICameraComponent3D;
// Forward END

ZN_INTERFACE ZN_API ILoadable
{
	enum class ZN_API ELoadableState : uint32
	{
		Created = 0,
		Loading,
		Loaded,
		Unloading,
		Unloaded,
		Deleted
	};

	virtual											~ILoadable() {}

	virtual bool									Load() = 0;
	virtual bool									Delete() = 0;
	virtual void                                    SetState(ELoadableState State) = 0;
	virtual ELoadableState                          GetState() const = 0;
	virtual void                                    AddDependense(std::shared_ptr<ILoadable> Loadable) = 0;
	virtual std::shared_ptr<ILoadable>              GetDependense() const = 0;
};

ZN_INTERFACE ZN_API	ILoader 
	: public IManager
{
	ZN_OBJECTCLASS(cLoader)

	virtual ~ILoader() {}

	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual void SetCamera(std::shared_ptr<ICameraComponent3D> _camera) = 0;

	virtual void AddToLoadQueue(const std::weak_ptr<ILoadable>& LoadableItem) = 0;
	virtual void AddToDeleteQueue(const std::shared_ptr<ILoadable>& LoadableItem) = 0;
};

const uint32 cBufferBindingSignature = 0x00100000;
const uint32 cInputSemanticSignature = 0x00100001;

