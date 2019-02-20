#pragma once
#include <windows.h>
#include <windowsx.h>
#include <string>

#include "services/Timer.h"

class IRenderer;
class IScene;

class CAppBase
{
public:
	virtual bool Init() = 0;
	virtual void OnResize() = 0;
	virtual void UpdateScene(float dt) = 0;
	virtual void DrawScene() = 0;

protected:
	IRenderer* m_Renderer = nullptr;
};


class CD3DApp : public CAppBase
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

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:
	bool InitMainWindow();
	bool InitRenderer();

	void CalculateFrameStats();

protected:
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