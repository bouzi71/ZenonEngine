#pragma once

#include "GameState.h"
#include "Loader.h"

class OW_ENGINE_API Application :
	public Object, 
	public IApplication, 
	public IGameStateManager
{
public:
	Application(std::shared_ptr<IBaseManager> BaseManager);
	Application(std::shared_ptr<IBaseManager> BaseManager, HINSTANCE hInstance);
	virtual ~Application();

	static IApplication& Get();

	// Default query
	int                             Run();
	void                            Stop();

	// Creators
	std::shared_ptr<IRenderDevice>  CreateRenderDevice(IRenderDevice::DeviceType DeviceType);
	std::shared_ptr<RenderWindow>   CreateRenderWindow(IWindowObject * WindowObject, bool vSync);

	

	// IApplication
	void                            DoBeforeRun() override;
	int                             DoRun() override;
	void                            DoAfterRun() override;

	std::shared_ptr<IRenderDevice>  GetRenderDevice() const override;
	void                            SetRenderDevice(std::shared_ptr<IRenderDevice> _renderDevice) override;
	std::shared_ptr<RenderWindow>   GetRenderWindow() const override;
	void                            SetRenderWindow(std::shared_ptr<RenderWindow> _renderWindow) override;

    CLoader*						GetLoader();

	// IGameStateManager
	void                            AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState) override;
	bool                            SetGameState(GameStatesNames::List _name) override;
	bool                            SetGameState(std::shared_ptr<IGameState> _newGameState) override;
	std::shared_ptr<IGameState>     GetGameState() override;

	// Application execution events
	Event                           Initialize;
	void                          OnInitialize(EventArgs& e);
	UpdateEvent                     Update;
	void                          OnUpdate(UpdateEventArgs& e);
	Event			                Terminate;
	void                          OnTerminate(EventArgs& e);
	Event                           Terminated;
	void                          OnTerminated(EventArgs& e);
	Event                           Exit;
	void                          OnExit(EventArgs& e);
	UserEvent                       UserEvent;
	void                          OnUserEvent(UserEventArgs& e);

	void                          OnRender(RenderEventArgs& e);
	void                          OnRenderUI(RenderEventArgs& e);

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

private:
	bool                            m_bIsInitialized;
	bool                            m_bIsRunning;

	// Handle to the module.
	CLoader                         m_Loader;
	HINSTANCE                       m_HINSTANCE;

	std::shared_ptr<IRenderDevice>  m_pRenderDevice;
	std::shared_ptr<RenderWindow>   m_pWindow;

	// IGameStateManager
	std::shared_ptr<IGameState>                                     m_CurrentGameState;
	std::map<GameStatesNames::List, std::shared_ptr<IGameState>>    m_GameStatesCollection;

	// Hold the connections
	Delegate<EventArgs>::FunctionDecl        InitializeConnection;
	Delegate<UpdateEventArgs>::FunctionDecl  UpdateConnection;
	Delegate<EventArgs>::FunctionDecl        TerminateConnection;
};

extern IApplication* _ApplicationInstance;

#define _RenderDevice Application::Get().GetRenderDevice()
