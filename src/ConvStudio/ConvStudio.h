#pragma once

#include "ui_ConvStudio.h"
#include "components\FIlesList.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>

#include "formats\rw\rwrs.h"

class CQConvStudio : public QWidget
{
	Q_OBJECT

public:
	CQConvStudio(QWidget *parent = Q_NULLPTR);

	bool OpenArchive(const QString& path);
	void SetStyle(const QString& qss_path);

private:
	Ui::CQConvStudioClass ui;
	CQFilesList* m_FilesList;
	QTabWidget* m_PreviewTabs;

	rw::rs::archive<rw::rs::EArchiveVer::VER1>* m_Archive;
};
