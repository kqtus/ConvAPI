#include "MainWindow.h"

#include "qmenubar.h"
#include "qtoolbar.h"
#include "qicon.h"
#include "qboxlayout.h"

#include "Widgets/D3DWidget.h"

#include "../services/Components/RwLevel/RwLevelManager.h"
#include "../services/Components/RwLevel/RwLevel.h"
#include "../services/Components/RwLevel/RwLevelDeserializer.h"

#include "../renderer/Renderables.h"

CQMainWindow::CQMainWindow()
{
	CreateWidgets();
	CreateToolbar();
	CreateActions();
	TestComponents();
}

CQMainWindow::~CQMainWindow()
{

}

void CQMainWindow::NewFile()
{

}

void CQMainWindow::Open()
{

}

bool CQMainWindow::Save()
{
	return false;
}

bool CQMainWindow::SaveAs()
{
	return false;
}

void CQMainWindow::About()
{

}

void CQMainWindow::DocumentWasModified()
{

}

void CQMainWindow::CreateToolbar()
{
	QMenu* file_menu = menuBar()->addMenu(tr("&File"));
	QMenu* edit_menu = menuBar()->addMenu(tr("&Edit"));
	QMenu* tools_menu = menuBar()->addMenu(tr("&Window"));
	QMenu* help_menu = menuBar()->addMenu(tr("&Help"));

	QToolBar* edit_tool_bar = new QToolBar("Edit");
	addToolBar(Qt::LeftToolBarArea, edit_tool_bar);

	edit_tool_bar->addAction(QIcon("C:/Users/elMarcoPL/Documents/Developer/ConvAPI/icons/icons8-drag-40.png"), "Move");
	edit_tool_bar->addAction(QIcon("C:/Users/elMarcoPL/Documents/Developer/ConvAPI/icons/icons8-plus-math-40.png"), "Add");
	edit_tool_bar->addAction(QIcon("C:/Users/elMarcoPL/Documents/Developer/ConvAPI/icons/icons8-enlarge-40"), "Scale");
	edit_tool_bar->addAction(QIcon("C:/Users/elMarcoPL/Documents/Developer/ConvAPI/icons/icons8-link-40.png"), "Connect");
	//QToolBar* file_tool_bar = addToolBar(tr("File"));
}

void CQMainWindow::CreateActions()
{
}

void CQMainWindow::CreateWidgets()
{
	auto layout = new QHBoxLayout();

	m_D3DWidget = new CQD3DWidget();

	//m_NodeEditor = new CQNodeEditorWidget();
	layout->addWidget(m_D3DWidget);
	layout->setMargin(0);

	m_D3DWidget->resize(500, 300);
	setCentralWidget(new QWidget);
	m_D3DWidget->InitRenderer();
	m_D3DWidget->show();
	centralWidget()->setLayout(layout);
}

void CQMainWindow::TestComponents()
{
	CRwLevelManager mgr;
	mgr.SetRootDir(L"D:/steam/steamapps/common/Grand Theft Auto Vice City/");
	bool level_loaded = mgr.LoadLevel("airport");

	std::vector<ILevel*> levels;
	mgr.GetLevels(levels);

	for (auto& lvl : levels)
	{
		std::vector<SPositionedMesh*> meshes;
		((CRwLevel*)lvl)->GetRwObjects(meshes);

		for (auto& pos_msh : meshes)
		{
			auto rw_mdl = new CRwModel(*pos_msh->mesh);
			rw_mdl->SetPos(pos_msh->pos);
			rw_mdl->SetRot(pos_msh->rot);
			m_D3DWidget->AddRenderable(rw_mdl);
		}
	}

	m_D3DWidget->ApplyRenderSource();
}