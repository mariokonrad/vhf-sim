#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "MainWindow.hpp"
#include "version.hpp"

int main(int argc, char ** argv)
{
	QApplication app(argc, argv);
	app.setApplicationName(simradrd68::project_name);
	app.setApplicationVersion(simradrd68::project_version);

	QTranslator translator;
	translator.load(
		"de_DE", app.applicationDirPath() + "/../share/" + simradrd68::project_name + "/lang");
	app.installTranslator(&translator);

	simradrd68::MainWindow win;
	win.show();

	return app.exec();
}
