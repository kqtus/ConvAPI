#pragma once

#include <qmainwindow.h>

class CQD3DWidget;

class CQMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	CQMainWindow();
	~CQMainWindow();

private slots:
	void NewFile();
	void Open();
	bool Save();
	bool SaveAs();
	void About();
	void DocumentWasModified();

protected:
	virtual void CreateToolbar();
	virtual void CreateActions();
	virtual void CreateWidgets();

	void TestComponents();

private:
	CQD3DWidget* m_D3DWidget = nullptr;
};
