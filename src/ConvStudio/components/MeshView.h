#pragma once
#include <qwindow.h>
#include <qglfunctions.h>
#include <qopenglpaintdevice.h>

class CQMeshView : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	CQMeshView(QWindow* parent = nullptr);
	virtual ~CQMeshView();

	virtual void Render(QPainter* painter);
	virtual void Render();

	virtual void Initialize();

public slots:
	void RenderLater();
	void RenderNow();

protected:
	bool event(QEvent* qvent) override;
	void exposeEvent(QExposeEvent* event) override;

private:
	QOpenGLContext * m_Context;
	QOpenGLPaintDevice* m_Device;
};