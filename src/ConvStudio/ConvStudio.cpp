#include "ConvStudio.h"
#include "qdockwidget.h"
#include "qtabwidget.h"
#include "qtextedit.h"
#include "qfile.h"
#include "qgridlayout.h"

#include "formats\rw\rwcore.h"
#include "formats\xml\node.h"

#include "converters\DffToXml.h"

CQConvStudio::CQConvStudio(QWidget *parent)
	: QWidget(parent)
{
	// http://doc.qt.io/qt-5/qtwidgets-mainwindows-dockwidgets-example.html
	
	QGridLayout* layout = new QGridLayout();
	
	m_FilesList = new CQFilesList(this);
	m_PreviewTabs = new QTabWidget(this);

	auto img_files = new QDockWidget(tr("IMG"), this);
	auto tab_preview = new QDockWidget(this);

	layout->addWidget(m_FilesList, 0, 0);
	layout->addWidget(m_PreviewTabs, 0, 1);
	
	setLayout(layout);
}

bool CQConvStudio::OpenArchive(const QString& path)
{
	using TInBinStream = in_stream<EStreamType::BINARY>;
	using TOutTextStream = out_stream<EStreamType::TEXT>;

	TInBinStream inp;

	inp.Open(path.toStdString().c_str());


	auto model = new CQFilesListModel(this);
	m_FilesList->setModel(model);

	m_Archive = new rw::rs::archive<rw::rs::EArchiveVer::VER1>();
	m_Archive->Read(inp);

	bool preview_added = false;
	for (auto& entry_hd : m_Archive->GetEntryHeaders())
	{
		std::string file_name((char*)entry_hd->file_name);
		
		QVariant entry_data;
		model->AddEntry(QString::fromStdString(file_name), entry_data);

		if (file_name.find(".dff") != std::string::npos && !preview_added)
		{
			preview_added = true;
			auto dff = new rw::core::clump();
			TInBinStream dff_stream;

			if (!m_Archive->GetFileStream(dff_stream, file_name))
			{
				break;
			}

			dff->Read(dff_stream);

			auto node = CConverter::From<rw::core::clump*, xml::node*>(dff);

			auto text_edit = new QTextEdit(m_PreviewTabs);
			text_edit->setText(QString::fromStdString(node->ToString()));
			m_PreviewTabs->addTab(qobject_cast<QWidget*>(text_edit), QString::fromStdString(file_name));
			
		}
	}
	
	return true;
}

void CQConvStudio::SetStyle(const QString& qss_path)
{
	QFile qss_file(qss_path);
	
	if (!qss_file.open(QFile::ReadOnly))
	{
		return;
	}

	QString style_sheet = QLatin1String(qss_file.readAll());
	setStyleSheet(style_sheet);
	update();
}