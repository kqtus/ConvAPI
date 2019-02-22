#pragma once
#include <qwidget.h>

class IRenderer;
class CScene;

class CQD3DWidget : public QWidget
{
	Q_OBJECT 
public:
	CQD3DWidget(QWidget* parent = nullptr)
		: QWidget(parent)
	{ 
		startTimer(1);
	}

	virtual QPaintEngine* paintEngine() const override { return nullptr; }

	virtual void timerEvent(QTimerEvent* event) override;

	virtual void wheelEvent(QWheelEvent* event) override;
	virtual void keyPressEvent(QKeyEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void dragMoveEvent(QDragMoveEvent* event) override;

	virtual void InitRenderer();
	virtual void OnFrame();

protected:
	IRenderer* m_Renderer = nullptr;
	CScene* m_MainScene = nullptr;

	int m_LastMouseX = 0;
	int m_LastMouseY = 0;
};