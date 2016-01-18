// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QStandardPaths>
#include <QCommandLineParser>
#include "MainWindow.hpp"
#include "System.hpp"
#include "version.hpp"
#include "engine/MMSI.hpp"

int main(int argc, char ** argv)
{
	// application
	QApplication app(argc, argv);
	app.setApplicationName(simradrd68::project_name);
	app.setApplicationVersion(simradrd68::project_version);

	// command line arguments

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addOption({"mmsi", "Specifies a non-modifable MMSI for the VHF", "mmsi"});
	parser.process(app);

	// configuration
	QSettings config(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/vhf.conf",
		QSettings::IniFormat);
	simradrd68::System::init(&config);

	if (parser.isSet("mmsi")) {
		using namespace simradrd68;
		System::fix_vhf_mmsi(engine::MMSI{parser.value("mmsi").toStdString()});
	}

	// internationalization
	QTranslator translator;
	translator.load(simradrd68::System::lang(),
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
