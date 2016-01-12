#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include "MainWindow.hpp"
#include "System.hpp"
#include "version.hpp"

int main(int argc, char ** argv)
{
	// application
	QApplication app(argc, argv);
	app.setApplicationName(simradrd68::project_name);
	app.setApplicationVersion(simradrd68::project_version);

	// configuration
	QSettings config(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/vhf.conf",
		QSettings::IniFormat);
	simradrd68::System::init(&config);
	simradrd68::System::load();

	// internationalization
	QTranslator translator;
	translator.load(simradrd68::System::lang().c_str(),
		app.applicationDirPath() + "/../share/" + simradrd68::project_name + "/lang");
	app.installTranslator(&translator);

	// run the application
	simradrd68::MainWindow win;
	win.show();
	auto rc = app.exec();

	// cleanup
	simradrd68::System::save();
	return rc;
}
