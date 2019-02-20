#pragma once
#include <windows.h>
#include <windowsx.h>
#include <string>

#include "services/Timer.h"
#include "IApp.h"

class IScene;

class CD3DApp : public IApp
{
public:
	CD3DApp(HINSTANCE hInstance);
	virtual ~CD3DApp();

	HINSTANCE GetInstance() const;
	HWND GetMainWindowHandler() const;
	float GetAspectRatio() const;

	int Run();

	virtual bool Init() override;
	virtual void OnResize() override;
	virtual void UpdateScene(float dt) override;
	virtual void DrawScene() override;
	virtual IRenderer* GetRenderer() const override;

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:
	bool InitMainWindow();
	bool InitRenderer();

	void CalculateFrameStats();

protected:
	IRenderer* m_Renderer = nullptr;

	HINSTANCE m_AppInstance;
	HWND m_MainWnd;

	bool m_AppPaused = false;
	bool m_Minimized = false;
	bool m_Maximized = false;
	bool m_Resizing = false;

	std::wstring m_MainWindowCaption;

	int m_ClientWidth = 800;
	int m_ClientHeight = 600;

	CTimer m_Timer;
	IScene* m_MainScene = nullptr;

	POINT m_LastMousePos;
};