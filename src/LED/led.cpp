#include <qapplication.h>
#include "MainWindow.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	CQMainWindow main_wnd;

	main_wnd.setMinimumWidth(640);
	main_wnd.setMinimumHeight(480);
	main_wnd.show();

	return app.exec();
}