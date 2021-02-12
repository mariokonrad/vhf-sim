#include "platform.hpp"
#include <QCoreApplication>

namespace vhfsim
{
QString platform_translations_path(const QCoreApplication & app)
{
	return app.applicationDirPath() + "/../translations/";
}

QString platform_resource_path(const QCoreApplication & app)
{
	return app.applicationDirPath() + "/../Resources/";
}
}

