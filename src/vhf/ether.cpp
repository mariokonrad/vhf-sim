// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include <QApplication>
#include <QTranslator>
#include <QStandardPaths>
#include "etherwindow.hpp"
#include "version.hpp"
#include "System.hpp"

int main(int argc, char ** argv)
{
	// application
	QApplication app(argc, argv);
	app.setApplicationName(QObject::tr("Ether"));
	app.setApplicationVersion(simradrd68::project_version);

	// internationalization
	QTranslator translator;
	translator.load(simradrd68::System::lang(),
		app.applicationDirPath() + "/../share/" + simradrd68::project_name + "/lang");
	app.installTranslator(&translator);

	simradrd68::EtherWindow win(nullptr);
	win.show();

	return app.exec();
}
