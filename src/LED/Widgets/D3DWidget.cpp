#include "D3DWidget.h"
#include <qcoreevent.h>
#include <qevent.h>

#include "../../renderer/Renderer/D3DX11Renderer.h"

#include "../../renderer/Scene.h"
#include "../../renderer/Renderables.h"
#include "../../formats/rw/rwcore.h"

void CQD3DWidget::resizeEvent(QResizeEvent* event)
{
	__super::resizeEvent(event);

	m_Renderer->SetWindowWidth(width());
	m_Renderer->SetWindowHeight(height());
	m_Renderer->OnResize();
}

void CQD3DWidget::timerEvent(QTimerEvent* event)
{
	OnFrame();
}

void CQD3DWidget::wheelEvent(QWheelEvent* event)
{
	__super::wheelEvent(event);
}

void CQD3DWidget::keyPressEvent(QKeyEvent* event)
{
	__super::keyPressEvent(event);
}

void CQD3DWidget::mousePressEvent(QMouseEvent* event)
{
	__super::mousePressEvent(event);
}

void CQD3DWidget::mouseReleaseEvent(QMouseEvent* event)
{
	__super::mouseReleaseEvent(event);
}

void CQD3DWidget::mouseMoveEvent(QMouseEvent* event)
{
	__super::mouseMoveEvent(event);

	int x = event->localPos().x();
	int y = event->localPos().y();

	{
		float dx = 0.030f * static_cast<float>(x - m_LastMouseX);
		float dy = 0.030f * static_cast<float>(y - m_LastMouseY);

		m_Renderer->Move(dx, dy);
	}

	m_LastMouseX = x;
	m_LastMouseY = y;

	event->accept();
}

void CQD3DWidget::dragMoveEvent(QDragMoveEvent* event)
{
	__super::dragMoveEvent(event);
}


void CQD3DWidget::InitRenderer()
{
	m_Renderer = new CD3DX11Renderer((HWND)this->winId());
	m_Renderer->SetWindowWidth(600);
	m_Renderer->SetWindowHeight(600);
	m_Renderer->Init();
	m_Renderer->OnResize();

	m_MainScene = new CScene();

	// Test: Displaying simple objs on scene
	auto read_model = [&](auto path, vec3<float> pos) -> CRwModel*
	{
		in_stream<EStreamType::BINARY> dff_stream;
		if (dff_stream.Open(path))
		{
			auto test_mdl = new CRwModel();
			test_mdl->Read(dff_stream);
			test_mdl->SetPos(pos);
			dff_stream.Close();

			return test_mdl;
		}

		return nullptr;
	};

	auto read_and_add_mdl = [&](auto path, vec3<float> pos)
	{
		if (auto mdl = read_model(path, pos))
		{
			m_MainScene->AddObject(mdl);
		}
	};

	read_and_add_mdl("assets\\dff\\sa\\sv_ground_04_sfs.dff", { 1.0f, 2.f, 0.f });
	read_and_add_mdl("assets\\dff\\vc\\dt_bowlsign.dff", { -2.0f, -2.f, 0.f });
	read_and_add_mdl("assets\\dff\\vc\\sabre.dff", { 10.0f, -2.f, 0.f });


}

void CQD3DWidget::OnFrame()
{
	m_Renderer->Update(0);
	m_Renderer->Render();
}

void CQD3DWidget::AddRenderable(CRwModel* model)
{
	m_MainScene->AddObject(model);
}

void CQD3DWidget::ApplyRenderSource()
{
	m_Renderer->AddRenderSource(m_MainScene);
}
