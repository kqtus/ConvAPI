#include "ConvStudio.h"
#include <QtWidgets/QApplication>

#include "DarkStyle.h"
#include "framelesswindow\framelesswindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CQConvStudio* w = new CQConvStudio;

	// style our application with custom dark style
	a.setStyle(new DarkStyle);

	// create frameless window (and set windowState or title)
	FramelessWindow framelessWindow;
	//framelessWindow.setWindowState(Qt::WindowMaximized);
	//framelessWindow.setWindowTitle("test title");


	// add the mainwindow to our custom frameless window
	framelessWindow.setContent(w);
	framelessWindow.showNormal();
	framelessWindow.setMinimumWidth(600);
	framelessWindow.setMinimumHeight(480);

	QString img_path = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Grand Theft Auto Vice City\\models\\gta3.img";

	w->OpenArchive(img_path);
	return a.exec();

}
