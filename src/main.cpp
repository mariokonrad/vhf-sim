#include <QApplication>
#include "MainWindow.hpp"
#include "version.hpp"

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Radio-Sim");
	app.setApplicationVersion(radiosim::version);

	radiosim::MainWindow win;
	win.show();

	return app.exec();
}
