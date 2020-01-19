#pragma once

//#define LOADER_ENABLED

class ZN_API CLoader 
	: public ILoader
{
public:
	CLoader();
	virtual ~CLoader();

	void AddToLoadQueue(std::shared_ptr<ILoadable> _item) override;
	void LoadAll() override;

	void AddToDeleteQueue(std::shared_ptr<ILoadable> _item) override;
	void DeleteAll() override;

	void SetCamera(std::shared_ptr<ICameraComponent3D> _camera);

	void LoaderThread(std::future<void> _promiseExiter);
	void SorterThread(std::future<void> futureObj);

	struct sortFunctor 
	{
		const std::shared_ptr<ICameraComponent3D>& camera;

		sortFunctor(const std::shared_ptr<ICameraComponent3D>& _camera)
			: camera(_camera)
		{ }

		bool operator()(const std::shared_ptr<ILoadable>& first, const std::shared_ptr<ILoadable>& second);
	};

private:
	const static uint32                    c_PoolSize = 8;

private:
	LockedQueue<std::shared_ptr<ILoadable>> m_QueueLoad;
	LockedQueue<std::shared_ptr<ILoadable>> m_QueueDelete;

	std::promise<void>					   m_Thread_Loader_Promise_Exiter[c_PoolSize];
	std::thread                            m_Thread_Loader[c_PoolSize];

	std::promise<void>					   m_Thread_Sorter_Promise;
	std::thread                            m_Thread_Sorter;

	std::shared_ptr<ICameraComponent3D>    m_Camera;
};