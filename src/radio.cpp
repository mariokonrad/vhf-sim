#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "MainWindow.hpp"
#include "version.hpp"

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName("Radio-Sim");
	app.setApplicationVersion(radiosim::project_version);

	QTranslator translator;
	translator.load(
		"de_DE", app.applicationDirPath() + "/../share/" + radiosim::project_name + "/lang");
	app.installTranslator(&translator);

	radiosim::MainWindow win;
	win.show();

	return app.exec();
}
