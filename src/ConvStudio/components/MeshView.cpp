#include "MeshView.h"

CQMeshView::CQMeshView(QWindow* parent)
	: QWindow(parent)
	, m_Context(nullptr)
	, m_Device(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
}

CQMeshView::~CQMeshView()
{

}

void CQMeshView::Render(QPainter* painter)
{
	Q_UNUSED(painter);
}

void CQMeshView::Render()
{
	if (!m_Device)
	{
		m_Device = new QOpenGLPaintDevice();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	m_Device->setSize(size());

	QPainter painter(m_Device);
	Render(&painter);
}

void CQMeshView::Initialize()
{

}

void CQMeshView::RenderLater()
{
	requestUpdate();
}

bool CQMeshView::event(QEvent* event)
{
	switch (event->type)
	{
	case QEvent::UpdateRequest:
		RenderNow();
		return true;
	default:
		return QWindow::event(event);
	}
}

void CQMeshView::exposeEvent(QExposeEvent* event)
{
	Q_UNUSED(event);

	if (isExposed())
	{
		RenderNow();
	}
}

